#ifndef LED_H
#define LED_H

#define NUM_LEDS 60
#define COLUM_LINE 8
#define DATA_PIN 6

#include "joystick.h"
#include <FastLED.h>

enum class PType
{
    Red,
    Blue,
};

PType from_jstype(JoyStickPType jspt)
{
    switch (jspt)
    {
    case JoyStickPType::Right:
        return PType::Red;
    case JoyStickPType::Left:
        return PType::Blue;
    }
}

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

enum class __CellState
{
    OCCUPIED,
    EMPTY
};

struct CellState
{
    __CellState state;
    union
    {
        PType player;
    };

    static CellState Occupied(PType pval)
    {
        CellState s;
        s.state = __CellState::OCCUPIED;
        s.player = pval;
        return s;
    }

    static CellState Empty()
    {
        CellState s;
        s.state = __CellState::EMPTY;
        return s;
    }
};

typedef struct Cell
{
    CellState state;
    int column;
    int line;
} Cell;

/*
class Cell
{
    CellState state;
    CRGB led_cell;
    int column;
    int line;
    Cell(CellState _state, CRGB _led_cell, int _column, int _line)
    {
        state = _state;
        led_cell = _led_cell;
        column = _column;
        line = _line;
    }
};
*/
// CRGB leds[COLUM_LINE][COLUM_LINE];
// Cell cells[COLUM_LINE][COLUM_LINE];
class LEDs
{
    CRGB(*leds)
    [COLUM_LINE];
    Cell (*cells)[COLUM_LINE];
    Cell cursor = (Cell){CellState::Occupied(PType::Red), 0, 0};
    PType player = PType::Red;
    LEDs(CRGB (*_leds)[COLUM_LINE], Cell (*_cells)[COLUM_LINE])
    {
        leds = _leds;
        cells = _cells;
    }

public:
    void setup_leds()
    {
        for (int i = 0; i < COLUM_LINE; i++)
        {
            FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds[i], NUM_LEDS);
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

    void handle_movement(JoyStick stick)
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

    void move_cursor()
    {
        leds[cursor.column][cursor.line] = ptype_to_CRGB(player);
        FastLED.show();
    }

    void gravity()
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

    void lop()
    {
        // leds[0] = CRGB::White;
        // FastLED.show();
        // delay(30);
        // leds[0] = CRGB::Black;
        // FastLED.show();
        // delay(30);
    }
};

#endif