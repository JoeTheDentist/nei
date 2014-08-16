#ifndef NEI_H
#define NEI_H

#include <vector>
#include <utility>
#include <map>

#include "../include/debug.h"

namespace nei
{

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
     */
    kNN(const Distance &d = Distance());

    /**
     * @brief kNN constructor, filling in the tarining data
     * @param begin start iterator
     * @param end end iterator
     * @param d distance functor to use for computations
     *
     * The provided iterator should contain std::pair<T, LabelClass>
     */
    template<class PairIterator>
    kNN(PairIterator begin, PairIterator end, const Distance &d = Distance());

    /**
     * @brief add_training_point
     * @param sample point to add
     * @param cls equivalence class of the training point
     */
    void add_training_point(T sample, LabelClass cls);

    /**
     * @brief classify
     * @param sample sample to classify
     * @param k number of neighbors to consider
     * @return equivalence class guessed for the given sample
     */
    LabelClass classify(T sample, unsigned int k);

private:
    // temp, naive approach
    std::vector<std::pair<T, LabelClass> > _store;
    Distance _dist;
};

template<class T, class Distance, class LabelClass>
kNN<T, Distance, LabelClass>::kNN(const Distance &d) : _store()
{
    _dist = d;
}

template<class T, class Distance, class LabelClass>
template<class PairIterator>
kNN<T, Distance, LabelClass>::kNN(PairIterator begin, PairIterator end, const Distance &d) : _store(begin, end)
{
    _dist = d;
}

template<class T, class Distance, class LabelClass>
void kNN<T, Distance, LabelClass>::add_training_point(T sample, LabelClass cls)
{
    _store.push_back(std::make_pair(sample, cls));
}

template<class T, class Distance, class LabelClass>
LabelClass kNN<T, Distance, LabelClass>::classify(T sample, unsigned int k)
{
    TRACE("Call to classify " << sample);
    // naive computation
    std::vector<std::pair<T, LabelClass> > closests;
    std::map<LabelClass, unsigned int> class_count;
    for (unsigned int i=0; i<k; ++i)
    {
        unsigned int min_dist = static_cast<unsigned int>(-1);
        typename std::vector<std::pair<T, LabelClass> >::iterator to_remove;
        std::pair<T, LabelClass> closest;
        for (typename std::vector<std::pair<T, LabelClass> >::iterator it = _store.begin(); it != _store.end(); ++it)
        {
            T cur_sample = it->first;
            TRACE("Treat " << it->first << ":" << it->second << " distance " << _dist(cur_sample, sample));
            unsigned int cur_dist = _dist(cur_sample, sample);
            if (cur_dist < min_dist)
            {
                min_dist = cur_dist;
                to_remove = it;
                closest = *it;
            }
        }
        if (min_dist == static_cast<unsigned int>(-1))
            break;
        _store.erase(to_remove);
        TRACE("Closest " << closest.first << ":" << closest.second);
        closests.push_back(closest);
        if (class_count.find(closest.second) == class_count.end())
            class_count[closest.second] = 1;
        else
            ++class_count[closest.second];
    }
    for (typename std::vector<std::pair<T, LabelClass> >::iterator it = closests.begin(); it != closests.end(); ++it)
    {
        _store.push_back(*it);
    }
    unsigned int max_occur = 0;
    LabelClass max_class;
    for (typename std::map<LabelClass, unsigned int>::iterator it = class_count.begin(); it != class_count.end(); ++it)
    {
        if (it->second > max_occur)
        {
            max_occur = it->second;
            max_class = it->first;
        }
    }
    return max_class;
}

}

#endif
