#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "led.h"

enum class StickStatus
{
    PositiveMovement,
    NegativeMovement,
    StaticMovement,
    Click,
};

enum class JoyStickPType {
    Left,
    Right,
};

class JoyStick
{
public:
    int stick_x;
    int stick_y;
    int button_click;
    JoyStickPType type;
    JoyStick(int x, int y, int button, JoyStickPType ty)
    {
        stick_x = x;
        stick_y = y;
        button_click = button;
        type = ty;
    }

    void setup_joystick()
    {

        pinMode(stick_x, INPUT);
        pinMode(stick_y, INPUT);
        pinMode(button_click, INPUT);
        digitalWrite(button_click, HIGH);
    }

    StickStatus handle_movements()
    {
        return (!digitalRead(button_click)) ? StickStatus::Click : handle_single_stick(analogRead(stick_x) * (5.0 / 1023.0));
    }

    /*
     * These follow cartesian coordinates logic
     *
     * 2.5 = Sesting
     * 5.0 = Negative
     * 0.0 = Positive
     */
    StickStatus handle_single_stick(float stick_val)
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
            StickStatus::StaticMovement;
        }
    }

    PType to_ptype() {
        switch (type) {
            case JoyStickPType::Right:
                return PType::Red;
            case JoyStickPType::Left:
                return PType::Blue;
        }
    }
};

#endif