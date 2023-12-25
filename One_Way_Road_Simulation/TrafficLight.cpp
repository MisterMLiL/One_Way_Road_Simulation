#include "RoadSection.h"

TrafficLight::TrafficLight(Color initialColor) : color(initialColor) {}

void TrafficLight::changeColor(TrafficLight::Color newColor) {
    color = newColor;
}

TrafficLight::Color TrafficLight::getColor() const {
    return color;
}

const wchar_t* TrafficLight::ToString() {
    const wchar_t* green = L"�������";
    const wchar_t* red = L"�������";

    if (color == Color::GREEN)
    {
        return green;
    }
    else
    {
        return red;
    }
}