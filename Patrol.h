//
// Created by hadar on 25/02/2026.
//

#ifndef INC_74_EX3_PATROL_H
#define INC_74_EX3_PATROL_H
#include "Ship.h"

// Patrol boat constants
static const double PATROL_MAX_SPEED = 15.0;  // nm/h
static const double PATROL_MAX_FUEL  = 900.0; // kl
static const int    PATROL_FUEL_RATE = 2;     // kl per nm

/**
 * Patrol boat: automatically visits all ports in a Hamiltonian circuit.
 * Destination port name is tracked by the Ship base class (destPortName).
 */
class Patrol : public Ship {
public:
    Patrol();
    ~Patrol() override = default;

    Patrol(const string& name, double corX, double corY, int resistance);

    // Refuel capped at PATROL_MAX_FUEL
    void refuel(double amount) override;

    // Pirate victory: patrol resistance -1; defeat: resistance +1
    void setAttackStat(bool victory) override;

    void printStatus() const override;
};

#endif //INC_74_EX3_PATROL_H

