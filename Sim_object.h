//
// Sim_object: abstract base class for all simulation entities (ships and ports).
// Provides a common interface for name, 2D location, and per-step update.
//

#ifndef INC_74_EX3_SIM_OBJECT_H
#define INC_74_EX3_SIM_OBJECT_H

#include <string>
#include <utility>
using namespace std;

// 2D location as (x, y) in nautical miles
using Location = pair<double, double>;

class Sim_object {
private:
    string name;

protected:
    double corX;
    double corY;

public:
    Sim_object(const string& name, double corX, double corY);
    virtual ~Sim_object() = default;

    // Returns the object's name
    const string& getName() const;

    // Returns the object's current (x, y) location
    Location getLocation() const;

    // Called once per time step by the Model
    virtual void update() = 0;

    // Print current status to stdout
    virtual void printStatus() const = 0;
};

#endif //INC_74_EX3_SIM_OBJECT_H
