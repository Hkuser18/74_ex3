//
// Created by hadar on 24/02/2026.
//

#ifndef INC_74_EX3_CRUISER_H
#define INC_74_EX3_CRUISER_H
#include "Ship.h"
#define SPD 75
#define CAP -10 //infinite fuel, so we define as -10 for differentiation.
#define RATE 0 // no fuel consumption, so we define as 0 for differentiation.



class Cruiser : public Ship {
public:
    Cruiser();
    ~Cruiser() override;
    Cruiser(string name, double corX, double corY, double speed, double heading, int fuel, int resistance);
};


#endif //INC_74_EX3_CRUISER_H