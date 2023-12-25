#include "RoadSection.h"

RoadSection::RoadSection() : light1(), light2() {
    sideMovement = false;
}

RoadSection::RoadSection(bool side) : light1(), light2() {
    sideMovement = side;
}

Car RoadSection::getFrontCar1() {
    return cars1.getCar();
};
Car RoadSection::getFrontCar2() {
    return cars2.getCar();
};

void RoadSection::addCar1(double time) {
    cars1.addCar(time);
};
void RoadSection::addCar2(double time) {
    cars2.addCar(time);
};

void RoadSection::deleteCar1() {
    cars1.removeCar();
};
void RoadSection::deleteCar2() {
    cars2.removeCar();
}

double RoadSection::getWaitingTime() {
    if (!sideMovement)
    {
        return cars1.getCar().getWaitingTime();
    }
    else
    {
        return cars2.getCar().getWaitingTime();
    }
}

void RoadSection::setWaitingTime(bool sideMovement, double wait) {
    if (!sideMovement)
    {
        cars1.setTimeFrontCar(wait);
    }
    else
    {
        cars2.setTimeFrontCar(wait);
    }
}

void RoadSection::deleteCar() {
    if (!sideMovement)
    {
        cars1.removeCar();
    }
    else
    {
        cars2.removeCar();
    }
}



bool RoadSection::carQueueIsEmpty() {
    return cars1.isEmpty() && cars2.isEmpty();
}

bool RoadSection::currentQueueEmpty()
{
    if (!sideMovement)
    {
        return cars1.isEmpty();
    }
    else
    {
        return cars2.isEmpty();
    }
}

void RoadSection::clearAllCars()
{
    cars1.clear();
    cars2.clear();
}

TrafficLight RoadSection::getLight1() { return light1; }
TrafficLight RoadSection::getLight2() { return light2; }

void RoadSection::setLight1(TrafficLight::Color color) { light1.changeColor(color); }
void RoadSection::setLight2(TrafficLight::Color color) { light2.changeColor(color); }

void RoadSection::switchLights() {
    if (light1.getColor() == TrafficLight::Color::RED && light2.getColor() == TrafficLight::Color::RED)
    {
        if (!sideMovement)
        {
            light1.changeColor(TrafficLight::Color::GREEN);
        }
        else
        {
            light2.changeColor(TrafficLight::Color::GREEN);
        }
    }
    else
    {
        light1.changeColor(TrafficLight::Color::RED);
        light2.changeColor(TrafficLight::Color::RED);
        sideMovement = !sideMovement;
    }
}

bool RoadSection::getSide() {
    return sideMovement;
}

void RoadSection::setSide(bool side)
{
    sideMovement = side;
}
