#include "joystick.h"
#include "common.h"
#include <Arduino.h>

JoyStick::JoyStick(int x, int y, int button, JoyStickPType ty)
{
    stick_x = x;
    stick_y = y;
    button_click = button;
    type = ty;
}

void JoyStick::setup_joystick()
{
    pinMode(stick_x, INPUT);
    pinMode(stick_y, INPUT);
    pinMode(button_click, INPUT);
    digitalWrite(button_click, HIGH);
}

StickStatus JoyStick::handle_movements()
{
    if (!digitalRead(button_click))
    {
        Serial.println("click");
        return StickStatus::Click;
    }
    return handle_single_stick(analogRead(stick_x) * (5.0 / 1023.0));
    // return (!digitalRead(button_click)) ? StickStatus::Click : handle_single_stick(analogRead(stick_x) * (5.0 / 1023.0));
}

// static = 1.62
//  negative = 3.33
//  positive = 0
StickStatus JoyStick::handle_single_stick(float stick_val)
{
    String name;
    float negative_movement_voltage;
    switch (this->to_ptype())
    {
    case PType::Blue:
        name = "blue";
        negative_movement_voltage = 3.2;
        break;
    case PType::Red:
        negative_movement_voltage = 4.9;
        name = "red";
        break;
    }
    if (stick_val > negative_movement_voltage)
    {
        Serial.print(name);
        Serial.println(" moved left");
        return StickStatus::NegativeMovement;
    }
    else if (stick_val < 1)
    {
        Serial.print(name);
        Serial.println(" moved right");
        return StickStatus::PositiveMovement;
    }
    else
    {
        return StickStatus::StaticMovement;
    }
}

PType JoyStick::to_ptype()
{
    switch (type)
    {
    case JoyStickPType::Right:
        return PType::Red;
    case JoyStickPType::Left:
        return PType::Blue;
    }
}