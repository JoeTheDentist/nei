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
    /**
     * @brief GuaranteeVictim default constructor
     * Should not be called by API
     */
    GuaranteeVictim() { GuaranteeVictim::bad = true; }

    /**
     * @brief GuaranteeVictim parametrized constructor
     * The only one we use for tests
     */
    GuaranteeVictim(unsigned int) {}

    /**
     * @brief GuaranteeVictim copy constructor
     * Should not be called by API
     */
    GuaranteeVictim(const GuaranteeVictim &) { GuaranteeVictim::bad = true; }

    /**
     * @brief GuaranteeVictim move constructor
     * Should not be called by API
     */
    GuaranteeVictim(GuaranteeVictim &&) { GuaranteeVictim::bad = true; }

    /**
     * @brief operator = copy assignment
     * @return
     * Should not be called by API
     */
    GuaranteeVictim& operator=(const GuaranteeVictim &) { GuaranteeVictim::bad = true; return *this; }

    /**
     * @brief operator = move assignment
     * @return
     * Should not be called by API
     */
    GuaranteeVictim& operator=(GuaranteeVictim &&) { GuaranteeVictim::bad = true; return *this; }

    // member to check validity of the object
    static bool bad;
};

class GuaranteeDistance
{
public:
    float operator() (const GuaranteeVictim&, const GuaranteeVictim&) { return 0; }
};

bool GuaranteeVictim::bad = false;

#endif // GUARANTEES_H
