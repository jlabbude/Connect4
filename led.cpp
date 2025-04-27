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
            cursor_YX_coords(7, cursor.line) = CRGB::White;
            cursor.column = 0;
            break;
        }
        cursor_YX_coords(cursor.column, cursor.line) = CRGB::White;
        cursor.column++;
        break;
    case StickStatus::NegativeMovement:
        if (cursor.column == 0)
        {
            cursor_YX_coords(0, cursor.line) = CRGB::White;
            cursor.column = 7;
            break;
        }
        cursor_YX_coords(cursor.column, cursor.line) = CRGB::White;
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
    cursor_YX_coords(cursor.column, cursor.line) = ptype_to_CRGB(player);
}

CRGB &LEDs::cursor_YX_coords(int x, int y)
{
    return crgb_leds[y * COLUM_LINE + x];
}

bool LEDs::win_condition() {
    // direction vectors: →, ↓, ↘, ↗
    const int dx[4] = { 1, 0, 1,  1 };
    const int dy[4] = { 0, 1, 1, -1 };

    for (int x = 0; x < COLUM_LINE; x++) {
        for (int y = 0; y < COLUM_LINE; y++) {
            // only start from occupied cells
            if (cells[x][y].state.state != __CellState::OCCUPIED) 
                continue;

            // grab which player sits here
            PType here = cells[x][y].state.player;

            // check each of the 4 directions
            for (int dir = 0; dir < 4; dir++) {
                int count = 1;

                // look ahead up to 3 more steps
                for (int step = 1; step < 4; step++) {
                    int nx = x + dx[dir] * step;
                    int ny = y + dy[dir] * step;

                    // out of bounds?
                    if (nx < 0 || ny < 0 || nx >= COLUM_LINE || ny >= COLUM_LINE)
                        break;

                    // same player?
                    if (cells[nx][ny].state.state == __CellState::OCCUPIED &&
                        cells[nx][ny].state.player == here) {
                        count++;
                    } else {
                        break;
                    }
                }

                // found four in a row!
                if (count >= 4) {
                    return true;
                }
            }
        }
    }

    // no winning line found
    return false;
}
