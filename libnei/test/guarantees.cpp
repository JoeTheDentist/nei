
#include <memory>
#include <gtest/gtest.h>

#include "../include/nei.h"
#include "guarantees.h"

TEST(NeiGuarantees, NoCopy_NoMove_Unique)
{
    GuaranteeVictim::copies = 0;
    GuaranteeVictim::moves = 0;
    nei::kNN<GuaranteeVictim, guarantee_t, GuaranteeDistance> classifier;
    classifier.add_training_point(NONE, std::unique_ptr<GuaranteeVictim>(new GuaranteeVictim(0)));
    classifier.classify(GuaranteeVictim(0), 2);
    EXPECT_EQ(GuaranteeVictim::copies, 0);
    EXPECT_EQ(GuaranteeVictim::moves, 0);
}

TEST(NeiGuarantees, NoCopy_NoMove_Variadic)
{
    GuaranteeVictim::copies = 0;
    GuaranteeVictim::moves = 0;
    nei::kNN<GuaranteeVictim, guarantee_t, GuaranteeDistance> classifier;
    classifier.add_training_point(NONE, 0);
    classifier.classify(GuaranteeVictim(0), 2);
    EXPECT_EQ(GuaranteeVictim::copies, 0);
    EXPECT_EQ(GuaranteeVictim::moves, 0);
}


// TODO, when move operator implemented, guarantee that there is no copy at move.
