#pragma once
#include <queue>

#include "Car.h"


class CarQueue {
private:

    std::queue<Car> carQueue;

public:
    void addCar(double time);

    Car getCar();

    void setTimeFrontCar(double time);

    void removeCar();

    void clear();
     
    bool isEmpty();
};