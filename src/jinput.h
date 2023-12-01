#pragma once

#include <stdbool.h>

typedef struct ButtonState {
    int key;
    bool pressed;
    bool is_down;
} ButtonState;

typedef struct GameInputs {
    ButtonState mouse1;
    ButtonState mouse2;
    ButtonState ctrl;
    ButtonState space;
    ButtonState left;
    ButtonState right;
    ButtonState up;
    ButtonState down;
} GameInputs;
