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
JoyStick *current = &joystick_right;

LEDs leds;

ThreadController controller = ThreadController();
Thread rightJoystickThread = Thread();
Thread leftJoystickThread = Thread();

void LEDs::gravity()
{
    for (; cursor.line < COLUM_LINE; cursor.line++)
    {
        if (cells[cursor.column][cursor.line].state.state == __CellState::EMPTY)
        {
            move_cursor();
            XY(cursor.column, cursor.line - 1) = CRGB::White;
            FastLED.show();
            delay(200);
            if (cursor.line + 1 >= 8 || cells[cursor.column][cursor.line + 1].state.state != __CellState::EMPTY)
            {
                cells[cursor.column][cursor.line].state = CellState::Occupied(player);
                break;
                return;
            }
        }
    }
}

JoyStick *LEDs::next_play(JoyStick *current)
{
    JoyStick *next_player;
    (current->type == JoyStickPType::Right) ? next_player = &joystick_left : next_player = &joystick_right;
    PType next_player_ptype = next_player->to_ptype();
    cursor = {CellState::Occupied(next_player_ptype), 0, 0};
    player = next_player_ptype;
    move_cursor();
    return next_player;
}

void handle_playing()
{
    StickStatus movement = current->handle_movements();
    leds.handle_movement(movement);
    FastLED.show();
    if (movement == StickStatus::Click)
    {
        leds.gravity();
        current = leds.next_play(current);
    }
    delay(200);
}

void setup_joysticks()
{
    joystick_right.setup_joystick();
    joystick_left.setup_joystick();
}

void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(crgb_leds, NUM_LEDS); // see next point
    FastLED.setBrightness(1);
    leds.setup_leds();
    FastLED.show();
    setup_joysticks();
    Serial.begin(9600);
}

void loop()
{
    handle_playing();
}
