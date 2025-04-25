#include "led.h"
#include "joystick.h"
#include <FastLED.h>

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

LEDs::LEDs(CRGB (*_leds)[COLUM_LINE], Cell (*_cells)[COLUM_LINE])
{
    leds = _leds;
    cells = _cells;
}

void LEDs::setup_leds()
{
    for (int i = 0; i < COLUM_LINE; i++)
    {
        FastLED.addLeds<WS2812, LED_PIN, RGB>(leds[i], NUM_LEDS);
    }

    for (int column = 0; column < COLUM_LINE; column++)
    {
        for (int line = 0; line < COLUM_LINE; line++)
        {
            cells[column][line] = (Cell){CellState::Empty(), column, line};
            leds[column][line] = CRGB::White;
        }
    }
    FastLED.setBrightness(50);
    FastLED.show();
}

void LEDs::handle_movement(JoyStick stick)
{
    player = stick.to_ptype();
    switch (stick.handle_movements())
    {
    case StickStatus::PositiveMovement:
        (cursor.column == 7) ? cursor.column = 0 : cursor.column++;
        break;
    case StickStatus::NegativeMovement:
        (cursor.column == 0) ? cursor.column = 7 : cursor.column--;
        break;
    case StickStatus::StaticMovement:
        break;
    case StickStatus::Click:
        if (cells[cursor.column][cursor.line].state.state == __CellState::EMPTY)
        {
            gravity();
        }
        return;
    }
    move_cursor();
}

void LEDs::move_cursor()
{
    leds[cursor.column][cursor.line] = ptype_to_CRGB(player);
    FastLED.show();
}

void LEDs::gravity()
{
    for (; cursor.line < COLUM_LINE; cursor.line++)
    {
        if (cells[cursor.column][cursor.line].state.state == __CellState::EMPTY)
        {
            move_cursor();
            if (cursor.line + 1 > 8 || cells[cursor.column][cursor.line + 1].state.state != __CellState::EMPTY)
            {
                cells[cursor.column][cursor.line].state = CellState::Occupied(player);
                break;
                return;
            }
        }
    }
}

void LEDs::lop()
{
    // todo
}