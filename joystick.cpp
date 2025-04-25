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
    return (!digitalRead(button_click)) ? StickStatus::Click : handle_single_stick(analogRead(stick_x) * (5.0 / 1023.0));
}

StickStatus JoyStick::handle_single_stick(float stick_val)
{
    if (stick_val > 2.6)
    {
        Serial.println(" moved left");
        return StickStatus::NegativeMovement;
    }
    else if (stick_val < 2.4)
    {
        Serial.println(" moved right");
        return StickStatus::PositiveMovement;
    }
    else
    {
        return StickStatus::StaticMovement;
    }
}

PType JoyStick::to_ptype() {
    switch (type) {
        case JoyStickPType::Right:
            return PType::Red;
        case JoyStickPType::Left:
            return PType::Blue;
    }
}