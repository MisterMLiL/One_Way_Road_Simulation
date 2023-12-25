#pragma once

#include "CarQueue.h"
#include "TrafficLight.h"

class RoadSection {
private:
    CarQueue cars1;
    CarQueue cars2;

    TrafficLight light1;
    TrafficLight light2;

    bool sideMovement;

public:
    RoadSection();
    RoadSection(bool side);

#pragma region Метод очередей автомобилей

    Car getFrontCar1();
    Car getFrontCar2();

    void addCar1(double time);
    void addCar2(double time);

    void deleteCar1();
    void deleteCar2();

    double getWaitingTime();

    void setWaitingTime(bool sideMovement, double wait);

    void deleteCar();

    bool carQueueIsEmpty();

    bool currentQueueEmpty();

    void clearAllCars();

#pragma endregion

#pragma region Методы светофоров

    TrafficLight getLight1();
    TrafficLight getLight2();

    void setLight1(TrafficLight::Color color);
    void setLight2(TrafficLight::Color color);

    void switchLights();

#pragma endregion

    bool getSide();

    void setSide(bool side);
};