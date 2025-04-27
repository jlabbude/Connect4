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

void win_animation(PType winner)
{
    CRGB winner_color = ptype_to_CRGB(winner);

    // Flash the entire board 5 times with the winner's color
    for (int flash = 0; flash < 5; flash++)
    {
        // Turn all LEDs to winner's color
        for (int i = 0; i < NUM_LEDS; i++)
        {
            crgb_leds[i] = winner_color;
        }
        FastLED.show();
        delay(300);

        // Turn all LEDs off (black)
        for (int i = 0; i < NUM_LEDS; i++)
        {
            crgb_leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(300);
    }

    // Spiral animation from the center outward
    int center = COLUM_LINE / 2;
    for (int radius = 1; radius <= COLUM_LINE; radius++)
    {
        for (int x = center - radius; x <= center + radius; x++)
        {
            for (int y = center - radius; y <= center + radius; y++)
            {
                // Check if position is at the edge of current radius and within bounds
                if ((x == center - radius || x == center + radius ||
                     y == center - radius || y == center + radius) &&
                    x >= 0 && x < COLUM_LINE && y >= 0 && y < COLUM_LINE)
                {
                    crgb_leds[y * COLUM_LINE + x] = winner_color;
                }
            }
        }
        FastLED.show();
        delay(150);
    }

    delay(1000);
    FastLED.show();
}

void LEDs::gravity()
{
    for (; cursor.line < COLUM_LINE; cursor.line++)
    {
        if (cells[cursor.column][cursor.line].state.state == __CellState::EMPTY)
        {
            move_cursor();
            cursor_YX_coords(cursor.column, cursor.line - 1) = CRGB::White;
            FastLED.show();
            delay(200);
            if (cursor.line + 1 >= 8 || cells[cursor.column][cursor.line + 1].state.state != __CellState::EMPTY)
            {
                cells[cursor.column][cursor.line].state = CellState::Occupied(player);
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

void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(crgb_leds, NUM_LEDS);
    FastLED.setBrightness(1);

    leds.setup_leds();
    FastLED.show();

    joystick_right.setup_joystick();
    joystick_left.setup_joystick();

    Serial.begin(9600);
}

void loop()
{
    StickStatus movement = current->handle_movements();
    leds.handle_movement(movement);
    FastLED.show();
    if (movement == StickStatus::Click)
    {
        leds.gravity();
        if (leds.win_condition()) {
            win_animation(current->to_ptype());
            leds.setup_leds();
            FastLED.show();
        }
        current = leds.next_play(current);
    }
    delay(200);
}
