//
// Created by hadar on 24/02/2026.
//

#include "Cruiser.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Default constructor
Cruiser::Cruiser()
    : Ship(), attackRange(0) {}


/**
 * Create a cruiser (pirate ship).
 * Fuel is not simulated, so fuel and fuelConsumption are set to 0.
 * @param name        Ship name
 * @param corX        Starting X coordinate
 * @param corY        Starting Y coordinate
 * @param force       Attack force value
 * @param attackRange Attack range in nm
 */
Cruiser::Cruiser(const string& name, double corX, double corY, int force, int attackRange)
    : Ship(name, corX, corY, 0.0, 0.0, 0.0, 0, force, CRUISER_MAX_SPEED, 0.0),
      attackRange(attackRange) {}

int Cruiser::getAttackRange() const { return attackRange; }

/**
 * Attack target ship.
 * Victory if cruiser force > target resistance.
 * Win:  target->setAttackStat(true),  cruiser force +1
 * Lose: target->setAttackStat(false), cruiser force -1
 * Either way: target is stopped.
 */
void Cruiser::attack(Ship* target) {
    bool victory = (attackStat > target->getAttackStat());
    target->setAttackStat(victory);
    if (victory) attackStat++;
    else         attackStat--;
    target->stop();
}

/**
 * Format:
 * Cruiser <name> at (<x>, <y>), force: <force>, <nav state>
 */
void Cruiser::printStatus() const {
    cout << fixed << setprecision(2);
    cout << "Cruiser " << getName()
         << " at (" << getCorX() << ", " << getCorY() << ")"
         << ", force: " << attackStat
         << ", " << navString()
         << "\n";
}