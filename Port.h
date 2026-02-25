//
// Created by hadar on 25/02/2026.
//

#ifndef INC_74_EX3_PORT_H
#define INC_74_EX3_PORT_H

#include "Sim_object.h"

/**
 * Port: a fixed simulation object with a fuel supply and a production rate.
 * Fuel is produced at fuelRate kl per time step (via update()).
 * Ships can request fuel via dispenseFuel() on a first-come-first-served basis.
 */
class Port : public Sim_object {
private:
    double fuel;      // current fuel stock in kl
    double fuelRate;  // kl produced per time step

public:
    Port(const string& name, double corX, double corY,
         double initialFuel, double fuelRate);

    // Produce fuel for this time step
    void update() override;

    // Print status: "Port <name> at position (<x>, <y>), Fuel available: <fuel> kl"
    void printStatus() const override;

    // Getters
    double getFuel()     const;
    double getFuelRate() const;

    /**
     * Dispense up to 'requested' kl of fuel (FCFS).
     * Returns the actual amount dispensed; deducts it from the stock.
     */
    double dispenseFuel(double requested);
};

#endif //INC_74_EX3_PORT_H
