#ifndef GUARANTEES_H
#define GUARANTEES_H

enum guarantee_t { NONE };

/**
 * @brief The GuaranteeVictim class
 * Class aimed to verify that the nei API does it job properly.
 * E.g. does not copy or move the object, does not call undesired
 * default constructor, assignments etc...
 */
class GuaranteeVictim
{
public:
    GuaranteeVictim(unsigned int) {}

    GuaranteeVictim(const GuaranteeVictim &) { ++GuaranteeVictim::copies; }

    GuaranteeVictim(GuaranteeVictim &&) { ++GuaranteeVictim::moves; }

    GuaranteeVictim& operator=(const GuaranteeVictim &) { ++GuaranteeVictim::copies; return *this; }

    GuaranteeVictim& operator=(GuaranteeVictim &&) { ++GuaranteeVictim::moves; return *this; }

    // members to check validity of the object
    static unsigned int copies;
    static unsigned int moves;
};

class GuaranteeDistance
{
public:
    float operator() (const GuaranteeVictim&, const GuaranteeVictim&) { return 0; }
};

unsigned int GuaranteeVictim::copies = 0;
unsigned int GuaranteeVictim::moves = 0;

#endif // GUARANTEES_H
