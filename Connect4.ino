#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include <FastLED.h>
#include "joystick.h"
#include "led.h"

#define RIGHT_Y A0
#define RIGHT_X A1
#define RIGHT_BUTTON 3

#define LEFT_Y A2
#define LEFT_X A3
#define LEFT_BUTTON 5

#define LED_COUNT 64
#define LED_PIN 4

JoyStick joystick_right(RIGHT_X, RIGHT_Y, RIGHT_BUTTON, JoyStickPType::Right);
JoyStick joystick_left(LEFT_X, LEFT_Y, LEFT_BUTTON, JoyStickPType::Left);

ThreadController controller = ThreadController();
Thread rightJoystickThread = Thread();
Thread leftJoystickThread = Thread();

CRGB leds[LED_COUNT];

void r_joystick_callback()
{
    joystick_right.handle_movements();
}

void l_joystick_callback()
{
    joystick_left.handle_movements();
}

void setup_joysticks() {
    joystick_right.setup_joystick();
    joystick_left.setup_joystick();

    rightJoystickThread.onRun(r_joystick_callback);
    rightJoystickThread.setInterval(50);

    leftJoystickThread.onRun(l_joystick_callback);
    leftJoystickThread.setInterval(50);

    controller.add(&rightJoystickThread);
    controller.add(&leftJoystickThread);
}

void setup()
{
    setup_joysticks();

    Serial.begin(9600);
}

void loop()
{
    controller.run();
}
