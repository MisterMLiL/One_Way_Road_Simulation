#include "CarQueue.h"

// ����� ���������� ���������� � ����� �������
void CarQueue::addCar(double time) {
    Car car = Car(time);
    carQueue.push(car);
}

// ����� ��������� ���������� �� ������ �������
Car CarQueue::getCar() {
    if (!carQueue.empty())
    {
        Car frontCar = carQueue.front();
        return frontCar;
    }
    else
    {
        // ���������� Car � ��������� ���������
        return Car(-1);
    }
}

void CarQueue::setTimeFrontCar(double time) {
    if (!carQueue.empty()) {
        // ���������� ������� ��������
        Car frontElement = carQueue.front();
        carQueue.pop();

        frontElement.setWaitingTime(time);

        carQueue.push(frontElement);

        for (size_t i = 1; i < carQueue.size(); ++i)
        {
            carQueue.push(carQueue.front());
            carQueue.pop();
        }
    }
}

// ����� �������� ���������� �� ������ �������
void CarQueue::removeCar() {
    if (!carQueue.empty())
    {
        carQueue.pop();
    }
}

void CarQueue::clear() {
    while (!carQueue.empty())
    {
        carQueue.pop();
    }
}

bool CarQueue::isEmpty() {
    return carQueue.empty();
}