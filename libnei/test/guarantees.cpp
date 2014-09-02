
#include <memory>
#include <gtest/gtest.h>

#include "../include/nei.h"
#include "guarantees.h"

TEST(NeiGuarantees, NoCopy_NoMove_NoAssignment)
{
    GuaranteeVictim::bad = false;
    nei::kNN<GuaranteeVictim, guarantee_t, GuaranteeDistance> classifier;
    classifier.add_training_point(std::unique_ptr<GuaranteeVictim>(new GuaranteeVictim(0)), NONE);
    classifier.classify(GuaranteeVictim(0), 2);
    EXPECT_FALSE(GuaranteeVictim::bad);
}

// TODO, when move operator implemented, guarantee that there is no copy at move.
