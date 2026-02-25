//
// Created by hadar on 25/02/2026.
//

#include "Port.h"
#include <iostream>
#include <iomanip>
using namespace std;

Port::Port(const string& name, double corX, double corY,
           double initialFuel, double fuelRate)
    : Sim_object(name, corX, corY), fuel(initialFuel), fuelRate(fuelRate) {}

// Produce fuel each time step
void Port::update() {
    fuel += fuelRate;
}

double Port::getFuel()     const { return fuel; }
double Port::getFuelRate() const { return fuelRate; }

// Dispense up to requested kl; return actual amount given
double Port::dispenseFuel(double requested) {
    double dispensed = (requested <= fuel) ? requested : fuel;
    fuel -= dispensed;
    return dispensed;
}

// "Port Nagoya at position (50.00, 5.00), Fuel available: 1001000.0 kl"
void Port::printStatus() const {
    cout << fixed << setprecision(1);
    cout << "Port " << getName()
         << " at position (" << corX << ", " << corY << ")"
         << ", Fuel available: " << fuel << " kl\n";
}
