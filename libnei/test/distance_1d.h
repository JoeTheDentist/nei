#ifndef DISTANCE_1D_H
#define DISTANCE_1D_H

#include <cmath>

class distance_1d
{
public:
    float operator()(float x, float y) const { return std::abs(x - y); }
};

enum class_t
{
    LEFT,
    RIGHT,
};

#endif // DISTANCE_1D_H
