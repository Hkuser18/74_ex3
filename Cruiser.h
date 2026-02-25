//
// Created by hadar on 24/02/2026.
//

#ifndef INC_74_EX3_CRUISER_H
#define INC_74_EX3_CRUISER_H
#include "Ship.h"

// Cruiser (pirate ship) constants
static const double CRUISER_MAX_SPEED = 75.0; // nm/h
// No fuel simulation for cruiser (fuel parameters unused)

/**
 * Pirate cruiser: attacks freighters and patrol boats within its attack range.
 * Cannot dock at ports. Fuel is not simulated.
 */
class Cruiser : public Ship {
private:
    int attackRange; // attack range in nm

public:
    Cruiser();
    ~Cruiser() override = default;

    // Create a cruiser with the given name, starting position, attack force, and range.
    Cruiser(const string& name, double corX, double corY, int force, int attackRange);

    // Get attack range
    int getAttackRange() const;

    /**
     * Attack a target ship that is in range.
     * Victory if this cruiser's force > target's resistance.
     * - Freighter victory: target loses all cargo, cruiser force +1.
     * - Freighter defeat:  no cargo change,       cruiser force -1.
     * - Patrol victory:    patrol resistance -1,   cruiser force +1.
     * - Patrol defeat:     patrol resistance +1,   cruiser force -1.
     * Either way, target is stopped (all movement cancelled).
     * @param target Pointer to the ship being attacked (must be in range).
     */
    void attack(Ship* target);

    // Print detailed status (no fuel shown for cruiser)
    void printStatus() const override;
};

#endif //INC_74_EX3_CRUISER_H

