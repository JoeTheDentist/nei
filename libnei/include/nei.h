#ifndef NEI_H
#define NEI_H

#include <vector>
#include <utility>
#include <map>
#include <memory>
#include <exception>
#include <iterator>
#include <algorithm>

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
 * @brief The NoTrainingDataException class
 * Exception to notify the user that he wants to classify
 * object without training data.
 */
class NoTrainingDataException : public std::exception {};

/**
 * @brief k nearest neighborgs algorithm.
 *
 * Class aimed to generacally support any type of data
 * given a distance relationship.
 * The distance class should be a functor on T class.
 * The label class is an enum used for classification.
 *
 * This class is not copyable, nor movable.
 */
template<class T, class LabelClass = int, class Distance = std::function<float(const T&, const T&)>>
class kNN
{
public:
    /**
     * @brief kNN constructor
     * @param d distance functor to use for computations
     * @complexity O(1)
     */
    kNN(const std::function<float(const T&, const T&)> &d = Distance());

    /**
     * @brief kNN constructor, filling in the tarining data
     * @param begin start iterator
     * @param end end iterator
     * @param d distance functor to use for computations
     * @complexity O(distance(begin, end))
     *
     * The provided iterator should contain std::pair<std::unique_ptr<T>, LabelClass>
     * Note ownership of contained objects are moved to the kNN object.
     * I.e. the container that manage the given iterators cannot be longer used.
     * E.g. for a std::vector, it is strongly adivised to call clear after the
     * construction of the kNN object.
     */
    template<class PairIterator>
    kNN(PairIterator begin, PairIterator end, const std::function<float(const T&, const T&)> &d = Distance());

    /* TODO implement move operations */
    kNN(const kNN&) = delete;
    kNN(kNN&&) = delete;
    kNN& operator=(const kNN&) = delete;
    kNN& operator=(kNN&&) = delete;

    /**
     * @brief add_training_point
     * @param sample point to add
     * @param cls equivalence class of the training point
     * @complexity O(1) (amortized)
     * @guarantee no copy, no move
     *
     * Taking ownership of the training point. Client cannot use any longer the contained
     * object.
     */
    void add_training_point(LabelClass cls, std::unique_ptr<T> sample);

    /**
     * @brief add_training_point
     * @param sample point to add
     * @param cls equivalence class of the training point
     * @complexity O(1) (amortized)
     * @guarantee no copy, no move
     */
    template<typename ...ConstructArgs>
    void add_training_point(LabelClass cls, ConstructArgs&&... args);

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
    LabelClass classify(const T &sample, unsigned int k, const std::function<float(float)> &wd = One());

private:
    // temp, naive approach
    std::vector<std::pair<std::unique_ptr<T>, LabelClass> > _store;
    std::function<float(const T&, const T&)> _dist;
};

template<class T, class LabelClass, class Distance>
kNN<T, LabelClass, Distance>::kNN(const std::function<float(const T&, const T&)> &d) : _store(), _dist(d) {}

template<class T, class LabelClass, class Distance>
template<class PairIterator>
kNN<T, LabelClass, Distance>::kNN(PairIterator begin, PairIterator end, const std::function<float(const T&, const T&)> &d) : _store(), _dist(d)
{
    for (PairIterator it = begin; it != end; ++it) { _store.push_back(std::move(*it)); }
}

template<class T, class LabelClass, class Distance>
void kNN<T, LabelClass, Distance>::add_training_point(LabelClass cls, std::unique_ptr<T> sample)
{
    _store.push_back(std::make_pair(std::move(sample), cls));
}

template<class T, class LabelClass, class Distance>
template<typename ...ConstructArgs>
void kNN<T, LabelClass, Distance>::add_training_point(LabelClass cls, ConstructArgs&&... args)
{
    _store.push_back(std::make_pair(std::unique_ptr<T>(new T(std::forward<ConstructArgs>(args)...)), cls));
}

template<class T, class LabelClass, class Distance>
LabelClass kNN<T, LabelClass, Distance>::classify(const T &sample, unsigned int k, const std::function<float(float)> &wd)
{
    // naive computation with vector
    TRACE("Call to classify " << sample);
    if (_store.empty())
        throw NoTrainingDataException();

    std::map<LabelClass, float> class_weights;
    for (unsigned int i=0; i<k; ++i)
    {
        // skipping if we don't have enough training points
        // we know that we have at least one anyway
        if (i >= _store.size())
            break;

        // find current closest
        unsigned int closest_index = 0;
        float closest_distance = _dist(*_store[0].first, sample);
        for (unsigned int j=0; j<_store.size() - i; ++j)
        {
            float curr_dist = _dist(*_store[j].first, sample);
            if (curr_dist < closest_distance)
            {
                closest_distance = curr_dist;
                closest_index = j;
            }
        }

        // save closest
        LabelClass closest_class = _store[closest_index].second;
        if (class_weights.find(closest_class) == class_weights.end())
            class_weights[closest_class] = wd(closest_distance);
        else
            class_weights[closest_class] += wd(closest_distance);

        // put back to don't treat it at next iteration
        // swap
        auto temp = std::move(_store[closest_index]);
        _store[closest_index] = std::move(_store[_store.size() - i - 1]);
        _store[_store.size() - i - 1] = std::move(temp);
    }

    float max_weight = class_weights.begin()->second;
    LabelClass max_class = class_weights.begin()->first;
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
