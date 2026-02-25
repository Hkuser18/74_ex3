//
// Model: Singleton that owns all simulation objects (ports and ships).
// Responsible for time advancement, object creation/lookup, and view support.
//

#ifndef INC_74_EX3_MODEL_H
#define INC_74_EX3_MODEL_H

#include <map>
#include <vector>
#include <memory>
#include <string>
#include "Port.h"
#include "Freighter.h"
#include "Patrol.h"
#include "Cruiser.h"
using namespace std;

/**
 * Model (Singleton): sole owner of all simulation objects.
 * Access via Model::get().
 */
class Model {
public:
    // Singleton access
    static Model& get();

    // Non-copyable
    Model(const Model&)            = delete;
    Model& operator=(const Model&) = delete;

    // --- Time ---
    int getTime() const;

    /**
     * Advance simulation by one hour:
     *   1. Update all ports (fuel production).
     *   2. Update all ships in insertion order (movement, fuel consumption).
     *   3. Increment the time counter.
     */
    void go();

    // --- Object creation (throws runtime_error if name already exists) ---
    void addPort(const string& name, double x, double y,
                 double initialFuel, double fuelRate);

    void addFreighter(const string& name, double x, double y,
                      int resistance, int maxContainers);

    void addPatrol(const string& name, double x, double y,
                   int resistance);

    void addCruiser(const string& name, double x, double y,
                    int force, int attackRange);

    // --- Typed lookup (throws runtime_error if not found) ---
    shared_ptr<Port>      getPort(const string& name)      const;
    shared_ptr<Freighter> getFreighter(const string& name) const;
    shared_ptr<Patrol>    getPatrol(const string& name)    const;
    shared_ptr<Cruiser>   getCruiser(const string& name)   const;

    // Returns a ship by name regardless of type (throws if not found)
    shared_ptr<Ship> getShip(const string& name) const;

    // Returns true if any object (ship or port) has this name
    bool nameExists(const string& name) const;

    // Returns true if a ship (any type) has this name
    bool shipExists(const string& name) const;

    // --- View support ---
    // Flat list of all simulation objects; the View iterates this to render the map
    vector<shared_ptr<Sim_object>> getAllObjects() const;

    // --- Status output ---
    // Print status of every object: ports first (map order), then ships (insertion order)
    void printStatus() const;

private:
    Model(); // creates mandatory Nagoya port

    int time; // current simulation time (hours)

    // One container per type for O(log n) typed lookup
    map<string, shared_ptr<Port>>      ports;
    map<string, shared_ptr<Freighter>> freighters;
    map<string, shared_ptr<Patrol>>    patrols;
    map<string, shared_ptr<Cruiser>>   cruisers;

    // Insertion-order list of ship names for deterministic update/status order
    vector<string> shipOrder;
};

#endif //INC_74_EX3_MODEL_H
