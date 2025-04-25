#ifndef COMMON_H
#define COMMON_H

enum class PType
{
    Red,
    Blue,
};

enum class StickStatus
{
    PositiveMovement,
    NegativeMovement,
    StaticMovement,
    Click,
};

enum class JoyStickPType {
    Left,
    Right,
};

PType from_jstype(JoyStickPType jspt);

#endif