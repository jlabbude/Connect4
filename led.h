#ifndef LED_H
#define LED_H

#define NUM_LEDS 64
#define COLUM_LINE 8
#define LED_PIN 6

#include "common.h"
#include <FastLED.h>

extern CRGB crgb_leds[NUM_LEDS];

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
    Cell cells[COLUM_LINE][COLUM_LINE];
    Cell cursor = (Cell){CellState::Occupied(PType::Red), 0, 0};
    PType player = PType::Red;

public:
    void setup_leds();
    void handle_movement(StickStatus movement);
    void move_cursor();
    void gravity();
    void lop();
    bool check_win();
    CRGB &cursor_YX_coords(int x, int y);
    bool win_condition();
    JoyStick *next_play(JoyStick *current);
};

#endif