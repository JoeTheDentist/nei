#ifndef NEI_H
#define NEI_H

#include <vector>
#include <utility>
#include <map>
#include <memory>

#include "../include/debug.h"

namespace nei
{

class One
{
public:
    float operator()(float) const { return 1; }
};

class WeightedDistance
{
public:
    float operator()(float d) const { return 1 / d; }
};

/**
 * @brief k nearest neighborgs algorithm.
 *
 * Class aimed to generacally support any type of data
 * given a distance relationship.
 * The distance class should be a functor on T class.
 * The label class is an enum used for classification.
 */
template<class T, class Distance, class LabelClass>
class kNN
{
public:
    /**
     * @brief kNN constructor
     * @param d distance functor to use for computations
     * @complexity O(1)
     */
    kNN(const Distance &d = Distance());

    /**
     * @brief kNN constructor, filling in the tarining data
     * @param begin start iterator
     * @param end end iterator
     * @param d distance functor to use for computations
     * @complexity O(distance(begin, end))
     *
     * The provided iterator should contain std::pair<T, LabelClass>
     */
    template<class PairIterator>
    kNN(PairIterator begin, PairIterator end, const Distance &d = Distance());

    /**
     * @brief add_training_point
     * @param sample point to add
     * @param cls equivalence class of the training point
     * @complexity O(1) (amortized)
     */
    void add_training_point(std::shared_ptr<T> sample, LabelClass cls);

    /**
     * @brief classify
     * @param sample sample to classify
     * @param k number of neighbors to consider
     * @param wd functor to treat distance of the k-neighbors. Defaut is constant function 1.
     * @return equivalence class guessed for the given sample
     * @complexity big theta(k x number of trained points)
     *
     * The "WeigthDistance" object is a functor aimed to treat
     * each of the k-neighbors according to its distance.
     */
    template<class WeightDistance = One>
    LabelClass classify(const T &sample, unsigned int k, const WeightDistance &wd = WeightDistance());

private:
    // temp, naive approach
    std::vector<std::pair<std::shared_ptr<T>, LabelClass> > _store;
    Distance _dist;
};

template<class T, class Distance, class LabelClass>
kNN<T, Distance, LabelClass>::kNN(const Distance &d) : _store(), _dist(d) {}

template<class T, class Distance, class LabelClass>
template<class PairIterator>
kNN<T, Distance, LabelClass>::kNN(PairIterator begin, PairIterator end, const Distance &d) : _store(begin, end), _dist(d) {}

template<class T, class Distance, class LabelClass>
void kNN<T, Distance, LabelClass>::add_training_point(std::shared_ptr<T> sample, LabelClass cls)
{
    _store.push_back(std::make_pair(sample, cls));
}

template<class T, class Distance, class LabelClass>
template<class WeightDistance>
LabelClass kNN<T, Distance, LabelClass>::classify(const T &sample, unsigned int k, const WeightDistance &wd)
{
    TRACE("Call to classify " << sample);
    // naive computation
    std::vector<std::pair<std::shared_ptr<T>, LabelClass> > closests;
    std::map<LabelClass, float> class_weights;
    for (unsigned int i=0; i<k; ++i)
    {
        // assuming that there is at least a tarining point available (e.g. k points total)
        // TODO proper treatment were we don't have enough points
        auto to_remove = _store.begin();
        float min_dist = _dist(*to_remove->first, sample);
        for (auto it = _store.begin(); it != _store.end(); ++it)
        {
            std::shared_ptr<T> cur_sample = it->first;
            TRACE("Treat " << it->first << ":" << it->second << " distance " << _dist(*cur_sample, sample));
            float cur_dist = _dist(*cur_sample, sample);
            if (cur_dist < min_dist)
            {
                min_dist = cur_dist;
                to_remove = it;
            }
        }
        TRACE("Closest " << to_remove->first << ":" << to_remove->second);
        if (class_weights.find(to_remove->second) == class_weights.end())
            class_weights[to_remove->second] = wd(to_remove->second);
        else
            class_weights[to_remove->second] += wd(to_remove->second);
        closests.push_back(*to_remove);
        _store.erase(to_remove);
    }
    for (auto it = closests.begin(); it != closests.end(); ++it)
    {
        _store.push_back(*it);
    }
    float max_weight = 0;
    // arbitrary value to avoid warning...
    LabelClass max_class = static_cast<LabelClass>(0);
    for (auto it = class_weights.begin(); it != class_weights.end(); ++it)
    {
        if (it->second > max_weight)
        {
            max_weight = it->second;
            max_class = it->first;
        }
    }
    return max_class;
}

}

#endif
