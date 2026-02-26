//
// Created by hadar on 25/02/2026.
//
#include "Model.h"
#include <stdexcept>
#include <iostream>
using namespace std;
// model is a singleton, so constructor is private and get() returns the single instance
Model& Model::get() {
    static Model instance;
    return instance;
}
// Private constructor: always creates the mandatory Nagoya port
Model::Model() : time(0) {
    addPort("Nagoya", 50.0, 5.0, 1000000.0, 1000.0);
}
//Time
int Model::getTime() const { return time; }
/**
 * Advance one time step:
 *   1. Update all ports (fuel production).
 *   2. Update all ships in insertion order (movement, fuel consumption).
 */
void Model::go() {
    for (auto& kv : ports)
        kv.second->update();
    for (const auto& name : shipOrder) {
        if (freighters.count(name))
            freighters.at(name)->update();
        else if (patrols.count(name))
            patrols.at(name)->update();
        else if (cruisers.count(name))
            cruisers.at(name)->update();
    }
    ++time;
}
//--Object creation--
// add new port with the given name, position, initial fuel, and fuel production rate
void Model::addPort(const string& name, double x, double y,
                    double initialFuel, double fuelRate) {
    if (nameExists(name))
        throw runtime_error("Name already exists: " + name);
    ports[name] = make_shared<Port>(name, x, y, initialFuel, fuelRate);
}
// add new freighter with the given name, starting position, resistance stat, and container capacity
void Model::addFreighter(const string& name, double x, double y,
                         int resistance, int maxContainers) {
    if (nameExists(name))
        throw runtime_error("Name already exists: " + name);
    freighters[name] = make_shared<Freighter>(name, x, y, resistance, maxContainers);
    shipOrder.push_back(name);
}
// add a patrol boat with the given name, starting position, and resistance stat
void Model::addPatrol(const string& name, double x, double y, int resistance) {
    if (nameExists(name))
        throw runtime_error("Name already exists: " + name);
    patrols[name] = make_shared<Patrol>(name, x, y, resistance);
    shipOrder.push_back(name);
}
// add a cruiser with the given name, starting position, attack force, and attack range
void Model::addCruiser(const string& name, double x, double y,
                       int force, int attackRange) {
    if (nameExists(name))
        throw runtime_error("Name already exists: " + name);
    cruisers[name] = make_shared<Cruiser>(name, x, y, force, attackRange);
    shipOrder.push_back(name);
}

// Typed lookup
// Returns a pointer to the object with the given name and type, or throws if not found, separated by specific type for better clarity.
shared_ptr<Port> Model::getPort(const string& name) const {
    auto it = ports.find(name);
    if (it == ports.end())
        throw runtime_error("No port named: " + name);
    return it->second;
}
shared_ptr<Freighter> Model::getFreighter(const string& name) const {
    auto it = freighters.find(name);
    if (it == freighters.end())
        throw runtime_error("No freighter named: " + name);
    return it->second;
}
shared_ptr<Patrol> Model::getPatrol(const string& name) const {
    auto it = patrols.find(name);
    if (it == patrols.end())
        throw runtime_error("No patrol boat named: " + name);
    return it->second;
}
shared_ptr<Cruiser> Model::getCruiser(const string& name) const {
    auto it = cruisers.find(name);
    if (it == cruisers.end())
        throw runtime_error("No cruiser named: " + name);
    return it->second;
}
shared_ptr<Ship> Model::getShip(const string& name) const {
    if (freighters.count(name)) return freighters.at(name);
    if (patrols.count(name))    return patrols.at(name);
    if (cruisers.count(name))   return cruisers.at(name);
    throw runtime_error("No ship named: " + name);
}
// Returns true if any object (ship or port) has this name
bool Model::nameExists(const string& name) const {
    return ports.count(name) || freighters.count(name)
        || patrols.count(name) || cruisers.count(name);
}
// Returns true if a ship (any type) has this name
bool Model::shipExists(const string& name) const {
    return freighters.count(name) || patrols.count(name) || cruisers.count(name);
}
// View support
// Returns all objects; View iterates to place 2-char labels on the map
vector<shared_ptr<Sim_object>> Model::getAllObjects() const {
    vector<shared_ptr<Sim_object>> result;
    for (auto& kv : ports)      result.push_back(kv.second);
    for (auto& kv : freighters) result.push_back(kv.second);
    for (auto& kv : patrols)    result.push_back(kv.second);
    for (auto& kv : cruisers)   result.push_back(kv.second);
    return result;
}
// Status
void Model::printStatus() const {
    for (auto& kv : ports)
        kv.second->printStatus();
    for (const auto& name : shipOrder) {
        if (freighters.count(name))
            freighters.at(name)->printStatus();
        else if (patrols.count(name))
            patrols.at(name)->printStatus();
        else if (cruisers.count(name))
            cruisers.at(name)->printStatus();
    }
}
