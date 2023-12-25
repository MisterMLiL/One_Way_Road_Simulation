#include "Car.h"

Car::Car(double time) {
	waitingTime = time;
}

double Car::getWaitingTime() const {
	return waitingTime;
}

void Car::setWaitingTime(double wait) {
	waitingTime = wait;
}