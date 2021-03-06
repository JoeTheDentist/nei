

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
    nei::kNN<int, class_t, distance_1d> test1;
    nei::kNN<int, class_t, distance_1d> test2( (distance_1d()) );
    std::vector<std::pair<std::unique_ptr<int>, class_t> > v;
    v.push_back(std::make_pair(std::unique_ptr<int>(new int(1)), RIGHT));
    nei::kNN<int, class_t, distance_1d> test3(v.begin(), v.end());
    v.clear();
}

TEST(kNN, Lambda)
{
    nei::kNN<int> test( [](int i, int j) -> float { return std::abs(i-j); } );
    test.add_training_point(RIGHT, 1);
    test.classify(1,1);
}

TEST(kNN, Classification1dSimple)
{
    nei::kNN<float, class_t, distance_1d> test;
    test.add_training_point(LEFT, std::unique_ptr<float>(new float(-1)));
    test.add_training_point(RIGHT, 1);
    EXPECT_EQ(test.classify(-0.5, 1), LEFT);
    EXPECT_EQ(test.classify(0.5, 1), RIGHT);
    EXPECT_EQ(test.classify(-2, 1), LEFT);
    EXPECT_EQ(test.classify(2, 1), RIGHT);
}

TEST(kNN, Classification1dMistake)
{
    nei::kNN<float, class_t, distance_1d> test;
    test.add_training_point(LEFT, -0.1);
    test.add_training_point(RIGHT, 0.2);
    test.add_training_point(RIGHT, 0.3);
    EXPECT_EQ(test.classify(-0.1, 1), LEFT);
    EXPECT_EQ(test.classify(-0.1, 3), RIGHT);
}

TEST(kNN, Classification1dExceptionNoData)
{
    nei::kNN<float, class_t, distance_1d> test;
    ASSERT_THROW(test.classify(-0.5, 1), nei::NoTrainingDataException);
}

TEST(kNN, Classification1dMaxK)
{
    nei::kNN<float, class_t, distance_1d> test;
    test.add_training_point(LEFT, -1);
    ASSERT_NO_THROW(test.classify(-0.5, 10));
}

TEST(kNN, Classification1dMultiple)
{
    nei::kNN<float, class_t, distance_1d> test;
    test.add_training_point(LEFT, -1);
    test.add_training_point(RIGHT, 1);
    test.add_training_point(LEFT, -0.3);
    test.add_training_point(RIGHT, 1.8);
    test.add_training_point(LEFT, -1.4);
    test.add_training_point(RIGHT, 1.2);
    EXPECT_EQ(test.classify(-0.5, 3), LEFT);
    EXPECT_EQ(test.classify(0.5, 3), RIGHT);
    EXPECT_EQ(test.classify(-2, 3), LEFT);
    EXPECT_EQ(test.classify(2, 3), RIGHT);
    EXPECT_EQ(test.classify(2, 3, nei::WeightedDistance()), RIGHT);
    class_t lambda_test = test.classify(2, 3, [](float d) -> float { return 1/d; });
    EXPECT_EQ(lambda_test, RIGHT);
}
