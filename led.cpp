#include "led.h"
#include "joystick.h"
#include <FastLED.h>

CRGB crgb_leds[NUM_LEDS];

CRGB ptype_to_CRGB(PType type)
{
    switch (type)
    {
    case PType::Red:
        return CRGB::Red;
    case PType::Blue:
        return CRGB::Blue;
    }
}

void LEDs::setup_leds()
{
    int idx = 0;
    for (int column = 0; column < COLUM_LINE; column++)
    {
        for (int line = 0; line < COLUM_LINE; line++)
        {
            cells[column][line] = {CellState::Empty(), column, line};
            crgb_leds[idx++] = CRGB::White;
        }
    }
}

void LEDs::handle_movement(StickStatus movement)
{
    switch (movement)
    {
    case StickStatus::PositiveMovement:
        if (cursor.column == 7)
        {
            XY(7, cursor.line) = CRGB::White;
            cursor.column = 0;
            break;
        }
        XY(cursor.column, cursor.line) = CRGB::White;
        cursor.column++;
        break;
    case StickStatus::NegativeMovement:
        if (cursor.column == 0)
        {
            XY(0, cursor.line) = CRGB::White;
            cursor.column = 7;
            break;
        }
        XY(cursor.column, cursor.line) = CRGB::White;
        cursor.column--;
        break;
    case StickStatus::StaticMovement:
        break;
    case StickStatus::Click:
        if (cells[cursor.column][cursor.line].state.state == __CellState::EMPTY)
            gravity();
        return;
    }
    move_cursor();
}

void LEDs::move_cursor()
{
    XY(cursor.column, cursor.line) = ptype_to_CRGB(player);
}

CRGB &LEDs::XY(int x, int y)
{
    return crgb_leds[y * COLUM_LINE + x];
}

void LEDs::lop()
{
    // todo
}