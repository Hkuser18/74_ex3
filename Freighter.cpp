//
// Created by hadar on 24/02/2026.
//

#include "Freighter.h"

#include <iostream>
#include <ostream>

Freighter::Freighter() {
}

Freighter::~Freighter() {
}

Freighter::Freighter(string name, double corX, double corY, double speed, double heading, int fuel, int resistance, int maxContainers) :
Ship(name, corX, corY, speed, heading, fuel, RATE, resistance), containers(0), maxContainers(maxContainers) {

}

int Freighter::getContainers() const {
}

int Freighter::getMaxContainers() const {
}

void Freighter::loadCargo(int amound) {
}

void Freighter::unloadCargo(int amount) {
    if (amount > containers) {
        cout << "Unloading all cargo"; //TODO: rephrase
        containers = 0;
    } else
        containers -= amount;
}

//access base class fuel setter. TODO: move to base class.
void Freighter::refuel(int amount) {
    setFuel(amount);
}


