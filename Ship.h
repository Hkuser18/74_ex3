//
// Created by hadar on 24/02/2026.
//

#ifndef INC_74_EX3_SHIP_H
#define INC_74_EX3_SHIP_H

#include "Sim_object.h"
#include <iostream>
using namespace std;

enum State {
    Stopped,
    Docked,
    DITW,   // Dead in the water
    Moving, // moving toward a named port or coordinate destination
    Course  // moving on a set compass course indefinitely
};

/**
 * Abstract base class for all ship types.
 * Manages movement, fuel consumption, heading, and combat stat.
 * Name and location are owned by Sim_object.
 */
class Ship : public Sim_object {
protected:
    int attackStat; // resistance (freighter/patrol) or attack force (cruiser)

private:
    double speed;            // current speed in nm/hr
    double heading;          // compass heading: 0=N, 90=E, 180=S, 270=W
    double fuel;             // current fuel in kl
    int    fuelConsumption;  // fuel burn in kl per nm travelled
    const double maxSpeed;   // type maximum speed
    const double maxFuel;    // type tank capacity
    State  state;            // current navigation state

    // Destination for Moving state
    double destX;
    double destY;
    string destPortName; // name of destination port if Moving to a port; else empty

public:
    Ship();
    ~Ship() override = default;

    // Parameterized constructor used by derived classes
    Ship(const string& name, double corX, double corY,
         double speed, double heading,
         double fuel, int fuelConsumption, int attackStat,
         double maxSpeed, double maxFuel);

    // Getters
    double getCorX()            const;
    double getCorY()            const;
    double getSpeed()           const;
    double getHeading()         const;
    double getFuel()            const;
    double getMaxFuel()         const;
    double getMaxSpeed()        const;
    int    getFuelConsumption() const;
    int    getAttackStat()      const;
    State  getState()           const;
    const string& getDestPortName() const;

    // Setters
    void setCorX(double corX);
    void setCorY(double corY);
    void setSpeed(double speed);
    void setHeading(double heading);
    void setFuel(double fuel);

    // Navigation commands
    void stop();
    void setCourse(double headingDeg, double spd);
    // Move toward specific coordinates (no named port)
    void setDestination(double cx, double cy, double spd);
    // Move toward a named port's coordinates (stores port name for status display)
    void setPortDestination(double cx, double cy, double spd, const string& portName);
    void changeState(State newState);

    // Combat
    virtual void setAttackStat(bool victory);

    // Sim_object interface
    // Advance one time step: move if not stopped/docked/DITW; consume fuel; go DITW if out of fuel
    void update() override;

    // Refuel: add amount capped at maxFuel (derived classes can override)
    virtual void refuel(double amount);

    // Print base status (overridden by each derived class)
    void printStatus() const override;

protected:
    // Build the navigation state string for use in derived printStatus()
    string navString() const;
};

#endif //INC_74_EX3_SHIP_H

