
#include <memory>
#include <gtest/gtest.h>

#include "../include/nei.h"
#include "guarantees.h"

TEST(NeiGuarantees, NoCopy_NoMove)
{
    GuaranteeVictim::copies = 0;
    GuaranteeVictim::moves = 0;
    nei::kNN<GuaranteeVictim, guarantee_t, GuaranteeDistance> classifier;
    classifier.add_training_point(std::unique_ptr<GuaranteeVictim>(new GuaranteeVictim(0)), NONE);
    classifier.classify(GuaranteeVictim(0), 2);
    EXPECT_EQ(GuaranteeVictim::copies, 0);
    EXPECT_EQ(GuaranteeVictim::moves, 0);
}

TEST(NeiGuarantees, NoCopy)
{
    GuaranteeVictim::copies = 0;
    GuaranteeVictim::moves = 0;
    nei::kNN<GuaranteeVictim, guarantee_t, GuaranteeDistance> classifier;
    classifier.add_training_point(GuaranteeVictim(0), NONE);
    classifier.classify(GuaranteeVictim(0), 2);
    EXPECT_EQ(GuaranteeVictim::copies, 0);
    EXPECT_NE(GuaranteeVictim::moves, 0);
}


// TODO, when move operator implemented, guarantee that there is no copy at move.
