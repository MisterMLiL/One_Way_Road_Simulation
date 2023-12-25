#pragma once

class TrafficLight {
public:
    enum class Color {
        GREEN,
        RED
    };

    TrafficLight(Color initialColor = Color::RED);

    void changeColor(Color newColor);

    Color getColor() const;

    const wchar_t* ToString();

private:
    Color color;
};