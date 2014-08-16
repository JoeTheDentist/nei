
#include <cmath>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

#include "../include/nei.h"

class _1d_distance
{
public:
    float operator()(float x, float y)
    {
        return ::abs(x - y);
    }
};

enum class_t
{
    LEFT,
    RIGHT,
};

TEST(Distance, functor)
{
    EXPECT_EQ(_1d_distance()(2,1), 1);
    EXPECT_EQ(_1d_distance()(1,2), 1);
}

TEST(kNN, Constructors)
{
    nei::kNN<int, _1d_distance, class_t> test1;
    nei::kNN<int, _1d_distance, class_t> test2( (_1d_distance()) );
    std::vector<std::pair<int, class_t> > v(4, std::pair<int, class_t>(1, LEFT));
    nei::kNN<int, _1d_distance, class_t> test3(v.begin(), v.end());
}

TEST(kNN, Classification1dSimple)
{
    nei::kNN<float, _1d_distance, class_t> test;
    test.add_training_point(-1, LEFT);
    test.add_training_point(1, RIGHT);
    EXPECT_EQ(test.classify(-0.5, 1), LEFT);
    EXPECT_EQ(test.classify(0.5, 1), RIGHT);
    EXPECT_EQ(test.classify(-2, 1), LEFT);
    EXPECT_EQ(test.classify(2, 1), RIGHT);
}

TEST(kNN, Classification1dMultiple)
{
    nei::kNN<float, _1d_distance, class_t> test;
    test.add_training_point(-1, LEFT);
    test.add_training_point(1, RIGHT);
    test.add_training_point(-0.3, LEFT);
    test.add_training_point(1.8, RIGHT);
    test.add_training_point(-1.4, LEFT);
    test.add_training_point(1.2, RIGHT);
    EXPECT_EQ(test.classify(-0.5, 1), LEFT);
    EXPECT_EQ(test.classify(0.5, 1), RIGHT);
    EXPECT_EQ(test.classify(-2, 1), LEFT);
    EXPECT_EQ(test.classify(2, 1), RIGHT);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
