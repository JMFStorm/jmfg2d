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
    ButtonState plus;
    ButtonState minus;
} GameInputs;
