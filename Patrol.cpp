//
// Created by hadar on 25/02/2026.
//

#include "Patrol.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Default constructor
Patrol::Patrol()
    : Ship() {}

// Destructor

/**
 * Create a patrol boat with full fuel tank.
 * @param name       Ship name
 * @param corX       Starting X coordinate
 * @param corY       Starting Y coordinate
 * @param resistance Resistance value against pirate attacks
 */
Patrol::Patrol(const string& name, double corX, double corY, int resistance)
    : Ship(name, corX, corY, 0.0, 0.0, PATROL_MAX_FUEL,
           PATROL_FUEL_RATE, resistance,
           PATROL_MAX_SPEED, PATROL_MAX_FUEL) {}

// Add fuel up to the tank capacity
void Patrol::refuel(double amount) {
    double newFuel = getFuel() + amount;
    if (newFuel > PATROL_MAX_FUEL) newFuel = PATROL_MAX_FUEL;
    setFuel(newFuel);
}

/**
 * On pirate attack result:
 *   victory (pirate won) -> patrol resistance decreases by 1
 *   defeat  (pirate lost) -> patrol resistance increases by 1
 */
void Patrol::setAttackStat(bool victory) {
    if (victory) attackStat--;
    else         attackStat++;
}

/**
 * Print patrol boat status.
 * Format:
 *   Patrol_boat <name> at (<x>, <y>), fuel: <fuel> kl, resistance: <res>, <nav state>
 */
void Patrol::printStatus() const {
    cout << fixed << setprecision(2);
    cout << "Patrol_boat " << getName()
         << " at (" << getCorX() << ", " << getCorY() << ")"
         << ", fuel: " << getFuel() << " kl"
         << ", resistance: " << attackStat
         << ", " << navString()
         << "\n";
}
