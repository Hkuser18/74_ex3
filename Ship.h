//
// Created by hadar on 24/02/2026.
//

#ifndef INC_74_EX3_SHIP_H
#define INC_74_EX3_SHIP_H
#include <string>
using namespace std;

enum State {
    Stopped,
    Docked,
    DITW,//Dead in the water
    Moving, //moving to destination.
    Course // moving on course
};

class Ship {
private:
    string name;
    double corX;
    double corY;
    double speed; // speed in nm (nautical miles) per hour/tick.
    double heading; // heading in degrees (0-360), where 0 is north, 90 is east, 180 is south, and 270 is west.
    int fuel;
    int fuelConsumption; // fuel consumption for distance travelled.
    int attackStat; //either resistance for freighter/patroller, or power for pirate.
    const double maxSpeed; // maximum speed for ship.
    const int maxFuel; // maximum fuel for ship.
    State state; // current state of the ship, can be Stopped, Docked, DITW (Dead in the water), or Moving.
public:
    Ship();

    virtual ~Ship();
    Ship(string name, double corX, double corY, double speed, double heading, int fuel, int fuelConsumption, int attackStat);
    string getName() const;
    double getCorX() const;
    double getCorY() const;
    double getSpeed() const;
    double getHeading() const;
    int getFuel() const;
    int getFuelConsumption() const;
    int getAttackStat() const;
    void setName(const string &name);
    void setCorX(double corX);
    void setCorY(double corY);
    void setSpeed(double speed);
    void setHeading(double heading);
    void setFuel(int fuel);
    void changeState(State newState);
    void refuel(int amount);
    void setCourse(double heading, double speed);
    void setDestination(double corX, double corY, double speed);
    virtual void setAttackStat(bool victory);
    void move();

};


#endif //INC_74_EX3_SHIP_H