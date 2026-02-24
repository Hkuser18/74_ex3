//
// Created by hadar on 24/02/2026.
//

#include "Ship.h"

#include <math.h>

// Default constructor, empty
Ship::Ship() : name(""), corX(0), corY(0), speed(0), heading(0), fuel(0), fuelConsumption(0), attackStat(0),
               maxSpeed(0),
               maxFuel(0) {
}

// Destructor, empty
Ship::~Ship() {
}

// Parameterized constructor, initializes all member variables with the provided values, will be used by derived classes.
Ship::Ship(string name, double corX, double corY, double speed, double heading, int fuel, int fuelConsumption,
    int attackStat) : name(name), corX(corX), corY(corY), speed(speed), heading(heading), fuel(fuel),
                      fuelConsumption(fuelConsumption), attackStat(attackStat), maxSpeed(0), maxFuel(0) {
}

string Ship::getName() const {
    return name;
}

double Ship::getCorX() const {
    return corX;
}

double Ship::getCorY() const {
    return corY;
}

double Ship::getSpeed() const {
    return speed;
}

double Ship::getHeading() const {
    return heading;
}

int Ship::getFuel() const {
        return fuel;
}

int Ship::getFuelConsumption() const {
    return fuelConsumption;
}

int Ship::getAttackStat() const {
    return attackStat;
}

void Ship::setName(const string &nameN) {
    this->name=nameN;
}

void Ship::setCorX(const double corXN) {
    this->corX=corXN;
}

void Ship::setCorY(const double corYN) {
    this->corY=corYN;
}

void Ship::setSpeed(double speedN) {
    this->speed=speedN;
}

void Ship::setHeading(double headingN) {
    this->heading=headingN;
}

void Ship::setFuel(int fuelN) {
        this->fuel=fuelN;
}

// Change the state of the ship to the new state. If the new state is Stopped, Docked, or DITW (Dead in the water), the speed is set to 0.
void Ship::changeState(State newState) {
    this->state = newState;
    if (newState == Stopped || newState == Docked || newState == DITW)
        this->speed = 0; // If the ship is stopped, docked, or dead in the water, its speed is set to 0.

}

/**
 *
 * @param corX - the X coordinate of the destination.
 * @param corY - the Y coordinate of the destination.
 * @param speed - the speed at which the ship will move towards the destination.
 * This method changes the state of the ship to Moving and sets the speed to the provided value
 * The heading will be calculated in the move method, based on the current coordinates and the destination coordinates, using basic trigonometry:
 * heading = atan2(corY - currentY, corX - currentX) * 180 / M_PI; // Convert heading from radians to degrees
 *
 */
void Ship::setDestination(double corX, double corY, double speed) {
    changeState(Moving);
    this->speed = speed;

}

// If victory is true, increase attackStat by 1, otherwise decrease it by 1. virtual to allow freighter to override.
void Ship::setAttackStat(bool victory) {
    if (victory)
        this->attackStat++;
    else
        this->attackStat--;
}

/***
 * Move the ship according to its speed and heading. The new coordinates are calculated using basic trigonometry:
 * newX = oldX + speed * cos(heading in radians)
 * newY = oldY + speed * sin(heading in radians)
 * The fuel is reduced by the fuel consumption for the distance traveled, which is equal to the speed.
 */
void Ship::move() {
    double headingInRadians = heading * M_PI / 180.0; // Convert heading from degrees to radians
    corX += speed * cos(headingInRadians); // Update X coordinate
    corY += speed * sin(headingInRadians); // Update Y coordinate
    if (fuel > 0) // Only reduce fuel if there is fuel left TODO: check rules for negative fuel, differentiate from cruiser.
        fuel -= static_cast<int>(speed) * fuelConsumption; // Reduce fuel by the fuel consumption for the distance traveled
}
