//
// Created by hadar on 24/02/2026.
//

#include "Freighter.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Default constructor
Freighter::Freighter()
    : Ship(), containers(0), maxContainers(0), unloadAmount(0) {}


/**
 * Create a freighter with full fuel tank.
 * @param name          Ship name
 * @param corX          Starting X coordinate
 * @param corY          Starting Y coordinate
 * @param resistance    Resistance value (attackStat in base)
 * @param maxContainers Maximum container capacity
 */
Freighter::Freighter(const string& name, double corX, double corY,
                     int resistance, int maxContainers)
    : Ship(name, corX, corY, 0.0, 0.0, FREIGHTER_MAX_FUEL,
           FREIGHTER_FUEL_RATE, resistance,
           FREIGHTER_MAX_SPEED, FREIGHTER_MAX_FUEL),
      containers(0), maxContainers(maxContainers), unloadAmount(0) {}

// Getters
int Freighter::getContainers()    const { return containers; }
int Freighter::getMaxContainers() const { return maxContainers; }
const string& Freighter::getLoadPort()   const { return loadPort; }
const string& Freighter::getUnloadPort() const { return unloadPort; }
int Freighter::getUnloadAmount()  const { return unloadAmount; }

// Fill cargo to capacity
void Freighter::loadCargo() {
    containers = maxContainers;
}

// Unload up to amount; warn if insufficient stock
void Freighter::unloadCargo(int amount) {
    if (amount > containers) {
        cout << "Warning: only " << containers << " containers on board, unloading all.\n";
        containers = 0;
    } else {
        containers -= amount;
    }
}

void Freighter::setLoadPort(const string& portName)  { loadPort = portName; }
void Freighter::clearLoadPort()                       { loadPort.clear(); }
void Freighter::setUnloadPort(const string& portName, int amount) {
    unloadPort   = portName;
    unloadAmount = amount;
}
void Freighter::clearUnloadPort() { unloadPort.clear(); unloadAmount = 0; }

// Refuel capped at tank capacity
void Freighter::refuel(double amount) {
    double newFuel = getFuel() + amount;
    if (newFuel > FREIGHTER_MAX_FUEL) newFuel = FREIGHTER_MAX_FUEL;
    setFuel(newFuel);
}

// Pirate victory: lose all cargo (resistance unchanged)
void Freighter::setAttackStat(bool victory) {
    if (victory) containers = 0;
}

/**
 * Format:
 * Freighter <name> at (<x>, <y>), fuel: <fuel> kl, resistance: <res>,
 *   <nav state>, Containers: <n>, <cargo dest info>
 *
 * Cargo dest info:
 *   "moving to loading destination"   — load port set, heading there or has both
 *   "moving to unloading destination" — only unload port set
 *   "no cargo destinations"           — neither set
 */
void Freighter::printStatus() const {
    // Cargo destination label
    string cargoDest;
    if (!loadPort.empty())
        cargoDest = "moving to loading destination";
    else if (!unloadPort.empty())
        cargoDest = "moving to unloading destination";
    else
        cargoDest = "no cargo destinations";

    cout << fixed << setprecision(2);
    cout << "Freighter " << getName()
         << " at (" << getCorX() << ", " << getCorY() << ")"
         << ", fuel: " << getFuel() << " kl"
         << ", resistance: " << attackStat
         << ", " << navString()
         << ", Containers: " << containers
         << ", " << cargoDest
         << "\n";
}
