//
// Created by hadar on 24/02/2026.
//

#ifndef INC_74_EX3_FREIGHTER_H
#define INC_74_EX3_FREIGHTER_H
#include "Ship.h"
#define SPD 40
#define VOL 500 // tank volume, 500k liters.
#define RATE 1 // fuel consumption for distance travelled, 1 kl (1000 liters) per nm.

class Freighter : public Ship {
private:
    int containers; // cargo volume in containers
    int maxContainers; // maximum cargo volume in units
public:
    Freighter();
    ~Freighter() override;
    Freighter(string name, double corX, double corY, double speed, double heading, int fuel, int resistance, int maxContainers);
    int getContainers() const;
    int getMaxContainers() const;
    void loadCargo(int amound);
    void unloadCargo(int amount);
    void refuel(int amount);
};


#endif //INC_74_EX3_FREIGHTER_H