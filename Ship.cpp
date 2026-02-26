//
// Created by hadar on 24/02/2026.
//

#include "Ship.h"
#include <cmath>
#include <iomanip>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Default constructor
Ship::Ship()
      : Sim_object("", 0.0, 0.0),
      attackStat(0), speed(0), heading(0),
      fuel(0), fuelConsumption(0), maxSpeed(0), maxFuel(0),
      state(Stopped), destX(0), destY(0) {}


// Parameterized constructor used by derived classes - initializes all members
Ship::Ship(const string& name, double corX, double corY,
           double speed, double heading,
           double fuel, int fuelConsumption, int attackStat,
           double maxSpeed, double maxFuel)
    : Sim_object(name, corX, corY),
      attackStat(attackStat), speed(speed), heading(heading),
      fuel(fuel), fuelConsumption(fuelConsumption),
      maxSpeed(maxSpeed), maxFuel(maxFuel),
      state(Stopped), destX(0), destY(0) {}

//getter, inline
double Ship::getCorX()            const { return corX; }
double Ship::getCorY()            const { return corY; }
double Ship::getSpeed()           const { return speed; }
double Ship::getHeading()         const { return heading; }
double Ship::getFuel()            const { return fuel; }
double Ship::getMaxFuel()         const { return maxFuel; }
double Ship::getMaxSpeed()        const { return maxSpeed; }
int    Ship::getFuelConsumption() const { return fuelConsumption; }
int    Ship::getAttackStat()      const { return attackStat; }
State  Ship::getState()           const { return state; }
const string& Ship::getDestPortName() const { return destPortName; }

// setters, inline
void Ship::setCorX(double v)    { corX = v; }
void Ship::setCorY(double v)    { corY = v; }
void Ship::setSpeed(double v)   { speed = v; }
void Ship::setHeading(double v) { heading = v; }
void Ship::setFuel(double v)    { fuel = v; }

// Stop, clear destination, and move to stop state
void Ship::stop() {
    destPortName.clear();
    destX = 0;
    destY = 0;
    changeState(Stopped);
}

// change state, zero speed if stopping/docking/DITW
void Ship::changeState(State newState) {
    state = newState;
    if (newState == Stopped || newState == Docked || newState == DITW)
        speed = 0;
}

// set course: clear destination and move to Course state
void Ship::setCourse(double headingDeg, double spd) {
    destPortName.clear();
    heading = headingDeg;
    speed   = spd;
    changeState(Course);
}

/**
 * Move toward specific coordinates (no named port).
 * Heading: 0=North(+Y), 90=East(+X). atan2(dx,dy) gives compass angle.
 */
void Ship::setDestination(double cx, double cy, double spd) {
    destPortName.clear();
    destX = cx;
    destY = cy;
    speed = spd;
    double dx = cx - corX;
    double dy = cy - corY;
    double h  = atan2(dx, dy) * 180.0 / M_PI;
    if (h < 0) h += 360.0;
    heading = h;
    changeState(Moving);
}

/**
 * Move toward a named port's coordinates.
 * Same math as setDestination but stores the port name for status display.
 */
void Ship::setPortDestination(double cx, double cy, double spd, const string& portName) {
    setDestination(cx, cy, spd);
    destPortName = portName;
}

// Adjust attackStat ±1
void Ship::setAttackStat(bool victory) {
    if (victory) attackStat++;
    else         attackStat--;
}

// Refuel: add amount capped at tank capacity
void Ship::refuel(double amount) {
    fuel += amount;
    if (fuel > maxFuel) fuel = maxFuel;
}

/**
 * update() — advance one time step (1 hour).
 * - Stopped / Docked / DITW: no movement.
 * - No fuel while moving: transition to DITW.
 * - Moving / Course: advance position, consume fuel.
 *   Ships only dock when explicitly commanded (dockAt in model/controller).
 */
void Ship::update() {
    if (state == Stopped || state == Docked || state == DITW)
        return;

    // Out of fuel → dead in the water
    if (fuelConsumption > 0 && fuel <= 0.0) {
        cout << getName() << " is out of fuel and is dead in the water.\n";
        changeState(DITW);
        return;
    }

    double rad  = heading * M_PI / 180.0;
    double step = speed; // distance travelled this step = speed * 1hr

    if (state == Moving) {
        // Cap step so we don't overshoot the destination
        double dx   = destX - corX;
        double dy   = destY - corY;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < step) step = dist;
        // Recompute heading toward destination each step
        double h = atan2(dx, dy) * 180.0 / M_PI;
        if (h < 0) h += 360.0;
        heading = h;
        rad = heading * M_PI / 180.0;
    }

    corX += step * sin(rad);
    corY += step * cos(rad);

    // Consume fuel proportional to distance travelled
    if (fuelConsumption > 0) {
        fuel -= step * fuelConsumption;
        if (fuel < 0) fuel = 0;
    }
}

/**
 * construct a string describing the current navigation state:
 * When Moving to a named port: "Moving to <portName> on course H deg, speed S nm/hr"
 * When Moving to coordinates:  "Moving to (<x>, <y>) on course H deg, speed S nm/hr"
 * When on Course:               "Moving on course H deg, speed S nm/hr"
 */
string Ship::navString() const {
    ostringstream oss;
    oss << fixed << setprecision(2);
    switch (state) {
        case Stopped: return "Stopped";
        case Docked:  return "Docked";
        case DITW:    return "Dead in the water";
        case Course:
            oss << "Moving on course " << heading
                << " deg, speed " << speed << " nm/hr";
            return oss.str();
        case Moving:
            if (!destPortName.empty())
                oss << "Moving to " << destPortName;
            else
                oss << "Moving to (" << destX << ", " << destY << ")";
            oss << " on course " << heading
                << " deg, speed " << speed << " nm/hr";
            return oss.str();
    }
    return "";
}

// Base printStatus
void Ship::printStatus() const {
    cout << fixed << setprecision(2);
    cout << getName() << " at (" << corX << ", " << corY << "), "
         << navString() << "\n";
}
