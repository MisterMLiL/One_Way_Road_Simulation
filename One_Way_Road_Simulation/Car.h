#pragma once
#include <cmath>
#include <random>

class Car {
public:
	Car(double time);

	double getWaitingTime() const;

	void setWaitingTime(double wait);

private:
	double waitingTime;
};