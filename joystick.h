#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "common.h"

class JoyStick
{
public:
    int stick_x;
    int stick_y;
    int button_click;
    JoyStickPType type;
    JoyStick(int x, int y, int button, JoyStickPType ty);
    void setup_joystick();
    StickStatus handle_movements();
    PType to_ptype();

private:
    StickStatus handle_single_stick(float stick_val);
};

#endif