

#include <cmath>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

#include "../include/nei.h"
#include "distance_1d.h"

TEST(Distance, functor)
{
    EXPECT_EQ(distance_1d()(2,1), 1);
    EXPECT_EQ(distance_1d()(1,2), 1);
}

TEST(kNN, Constructors)
{
    nei::kNN<int, distance_1d, class_t> test1;
    nei::kNN<int, distance_1d, class_t> test2( (distance_1d()) );
    /*std::vector<std::pair<int, class_t> > v(4, std::pair<int, class_t>(1, LEFT));
    nei::kNN<int, _1d_distance, class_t> test3(v.begin(), v.end());*/
}

TEST(kNN, Classification1dSimple)
{
    nei::kNN<float, distance_1d, class_t> test;
    test.add_training_point(std::shared_ptr<float>(new float(-1)), LEFT);
    test.add_training_point(std::shared_ptr<float>(new float(1)), RIGHT);
    EXPECT_EQ(test.classify(-0.5, 1), LEFT);
    EXPECT_EQ(test.classify(0.5, 1), RIGHT);
    EXPECT_EQ(test.classify(-2, 1), LEFT);
    EXPECT_EQ(test.classify(2, 1), RIGHT);
}

TEST(kNN, Classification1dException)
{
    nei::kNN<float, distance_1d, class_t> test;
    ASSERT_THROW(test.classify(-0.5, 1), nei::NoTrainingDataException);
}

TEST(kNN, Classification1dMaxK)
{
    nei::kNN<float, distance_1d, class_t> test;
    test.add_training_point(std::shared_ptr<float>(new float(-1)), LEFT);
    ASSERT_NO_THROW(test.classify(-0.5, 10));
}

TEST(kNN, Classification1dMultiple)
{
    nei::kNN<float, distance_1d, class_t> test;
    test.add_training_point(std::shared_ptr<float>(new float(-1)), LEFT);
    test.add_training_point(std::shared_ptr<float>(new float(1)), RIGHT);
    test.add_training_point(std::shared_ptr<float>(new float(-0.3)), LEFT);
    test.add_training_point(std::shared_ptr<float>(new float(1.8)), RIGHT);
    test.add_training_point(std::shared_ptr<float>(new float(-1.4)), LEFT);
    test.add_training_point(std::shared_ptr<float>(new float(1.2)), RIGHT);
    EXPECT_EQ(test.classify(-0.5, 3), LEFT);
    EXPECT_EQ(test.classify(0.5, 3), RIGHT);
    EXPECT_EQ(test.classify(-2, 3), LEFT);
    EXPECT_EQ(test.classify(2, 3), RIGHT);
    EXPECT_EQ(test.classify(2, 3, nei::WeightedDistance()), RIGHT);
}
