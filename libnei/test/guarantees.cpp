
#include <memory>
#include <gtest/gtest.h>

#include "../include/nei.h"
#include "guarantees.h"

TEST(NeiGuarantees, NoCopy_NoMove_NoAssignment)
{
    GuaranteeVictim::bad = false;
    nei::kNN<GuaranteeVictim, GuaranteeDistance, guarantee_t> classifier;
    classifier.add_training_point(std::unique_ptr<GuaranteeVictim>(new GuaranteeVictim(0)), NONE);
    classifier.classify(GuaranteeVictim(0), 2);
    EXPECT_FALSE(GuaranteeVictim::bad);
}
