#ifndef LED_H
#define LED_H

#define NUM_LEDS 60
#define COLUM_LINE 8
#define LED_PIN 6

#include "common.h"
#include <FastLED.h>

class JoyStick;

CRGB ptype_to_CRGB(PType type);

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

class LEDs
{
    CRGB(*leds)[COLUM_LINE];
    Cell (*cells)[COLUM_LINE];
    Cell cursor = (Cell){CellState::Occupied(PType::Red), 0, 0};
    PType player = PType::Red;

public:
    LEDs(CRGB (*_leds)[COLUM_LINE], Cell (*_cells)[COLUM_LINE]);
    void setup_leds();
    void handle_movement(JoyStick stick);
    void move_cursor();
    void gravity();
    void lop();
};

#endif