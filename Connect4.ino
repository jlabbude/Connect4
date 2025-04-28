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

#define RIGHT JoyStick(RIGHT_X, RIGHT_Y, RIGHT_BUTTON, JoyStickPType::Right)
#define LEFT JoyStick(LEFT_X, LEFT_Y, LEFT_BUTTON, JoyStickPType::Left)

void win_animation(CRGB winner_color)
{
    for (int flash = 0; flash < 5; flash++)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            crgb_leds[i] = winner_color;
        }
        FastLED.show();
        delay(300);

        for (int i = 0; i < NUM_LEDS; i++)
        {
            crgb_leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(300);
    }

    int center_start = 3;
    int center_end = 4;

    for (int i = 0; i < NUM_LEDS; i++)
    {
        crgb_leds[i] = CRGB::Black;
    }

    for (int x = center_start; x <= center_end; x++)
    {
        for (int y = center_start; y <= center_end; y++)
        {
            crgb_leds[y * COLUM_LINE + x] = winner_color;
        }
    }
    FastLED.show();
    delay(300);

    for (int radius = 1; radius <= 4; radius++)
    {
        int min_x = center_start - radius;
        int max_x = center_end + radius;
        int min_y = center_start - radius;
        int max_y = center_end + radius;

        min_x = max(min_x, 0);
        max_x = min(max_x, COLUM_LINE - 1);
        min_y = max(min_y, 0);
        max_y = min(max_y, COLUM_LINE - 1);

        for (int x = min_x; x <= max_x; x++)
        {
            if (min_y >= 0)
                crgb_leds[min_y * COLUM_LINE + x] = winner_color;
            if (max_y < COLUM_LINE)
                crgb_leds[max_y * COLUM_LINE + x] = winner_color;
        }

        for (int y = min_y + 1; y < max_y; y++)
        {
            if (min_x >= 0)
                crgb_leds[y * COLUM_LINE + min_x] = winner_color;
            if (max_x < COLUM_LINE)
                crgb_leds[y * COLUM_LINE + max_x] = winner_color;
        }

        FastLED.show();
        delay(200);
    }

    delay(1000);
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

void LEDs::next_play(JoyStick &current)
{
    // fixme: might not be ideal - might just convert each field manually in a switch_player method
    (current.type == JoyStickPType::Right) ? current = LEFT : current = RIGHT;
    player = current.to_ptype();
    cursor = {CellState::Occupied(player), 0, 0};
    move_cursor();
}

int main()
{
    JoyStick current = RIGHT;
    LEDs leds;

    FastLED.addLeds<WS2812, LED_PIN, GRB>(crgb_leds, NUM_LEDS);
    FastLED.setBrightness(1);
    leds.setup_leds();
    FastLED.show();

    JoyStick joystick_right(RIGHT_X, RIGHT_Y, RIGHT_BUTTON, JoyStickPType::Right);
    JoyStick joystick_left(LEFT_X, LEFT_Y, LEFT_BUTTON, JoyStickPType::Left);
    joystick_right.setup_joystick();
    joystick_left.setup_joystick();

    Serial.begin(9600);

    while (1)
    {
        StickStatus movement = current.handle_movements();
        leds.handle_movement(movement);
        FastLED.show();
        if (movement == StickStatus::Click)
        {
            leds.gravity();
            if (leds.win_condition())
            {
                win_animation(ptype_to_CRGB(current.to_ptype()));
                leds.setup_leds();
                FastLED.show();
            }
            leds.next_play(current);
        }
        delay(200);
    }

    return 0;
}
