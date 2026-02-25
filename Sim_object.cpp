//
// Created by hadar on 25/02/2026.
//
#include "Sim_object.h"
Sim_object::Sim_object(const string& name, double corX, double corY)
    : name(name), corX(corX), corY(corY) {}
const string& Sim_object::getName() const { return name; }
Location Sim_object::getLocation() const { return {corX, corY}; }
