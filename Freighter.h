//
// Created by hadar on 24/02/2026.
//

#ifndef INC_74_EX3_FREIGHTER_H
#define INC_74_EX3_FREIGHTER_H
#include "Ship.h"

// Freighter ship constants
static const double FREIGHTER_MAX_SPEED = 40.0;  // nm/h
static const double FREIGHTER_MAX_FUEL  = 500.0; // kl
static const int    FREIGHTER_FUEL_RATE = 1;     // kl per nm

/**
 * Cargo ship: moves containers between ports.
 * Fuel tank starts full on construction.
 */
class Freighter : public Ship {
private:
    int containers;    // current cargo count
    int maxContainers; // maximum capacity
    string loadPort;   // port name to load at (empty if none set)
    string unloadPort; // port name to unload at (empty if none set)
    int unloadAmount;  // containers to unload when arriving at unloadPort

public:
    Freighter();
    ~Freighter() override = default;

    Freighter(const string& name, double corX, double corY,
              int resistance, int maxContainers);

    // Getters
    int getContainers()    const;
    int getMaxContainers() const;
    const string& getLoadPort()   const;
    const string& getUnloadPort() const;
    int getUnloadAmount()  const;

    // Cargo operations (called by model when docked at the relevant port)
    void loadCargo();
    void unloadCargo(int amount);

    // Set cargo destinations (called by controller)
    void setLoadPort(const string& portName);
    void setUnloadPort(const string& portName, int amount);
    void clearLoadPort();
    void clearUnloadPort();

    // Refuel capped at FREIGHTER_MAX_FUEL
    void refuel(double amount) override;

    // On pirate victory: lose all containers (resistance unchanged)
    void setAttackStat(bool victory) override;

    void printStatus() const override;
};

#endif //INC_74_EX3_FREIGHTER_H

