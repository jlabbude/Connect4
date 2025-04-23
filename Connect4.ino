#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

#define RIGHT_Y A0
#define RIGHT_X A1
#define RIGHT_BUTTON 3

#define LEFT_Y A2
#define LEFT_X A3
#define LEFT_BUTTON 5

typedef enum StickStatus
{
    PositiveMovement,
    NegativeMovement,
    StaticMovement,
    Click,
} StickStatus;

class JoyStick
{
public:
    int stick_x;
    int stick_y;
    int button_click;
    String type;
    JoyStick(int x, int y, int button, String ty)
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
        if (!digitalRead(button_click))
        {
            Serial.print(type);
            Serial.println(" click");
            return Click;
        }
        return handle_single_stick(analogRead(stick_x) * (5.0 / 1023.0));
        // (!digitalRead(button_click)) ? Click : handle_single_stick(analogRead(stick_x) * (5.0 / 1023.0));
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
            Serial.print(type);
            Serial.println(" moved left");
            return NegativeMovement;
        }
        else if (stick_val < 2.4)
        {
            Serial.print(type);
            Serial.println(" moved right");
            return PositiveMovement;
        }
        else
        {
            StaticMovement;
        }
    }
};


JoyStick joystick_right(RIGHT_X, RIGHT_Y, RIGHT_BUTTON, "RIGHT");
JoyStick joystick_left(LEFT_X, LEFT_Y, LEFT_BUTTON, "LEFT");

ThreadController controller = ThreadController();
Thread rightJoystickThread = Thread();
Thread leftJoystickThread = Thread();

void callr()
{
    joystick_right.handle_movements();
}

void calll()
{
    joystick_left.handle_movements();
}

void setup()
{
    joystick_right.setup_joystick();
    joystick_left.setup_joystick();

    rightJoystickThread.onRun(callr);
    rightJoystickThread.setInterval(50);

    leftJoystickThread.onRun(calll);
    leftJoystickThread.setInterval(50);

    controller.add(&rightJoystickThread);
    controller.add(&leftJoystickThread);

    Serial.begin(9600);
}

void loop()
{
    controller.run();
}
