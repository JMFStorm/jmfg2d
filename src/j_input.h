#pragma once

typedef struct ButtonState {
    int key;
    int pressed;
    int is_down;
} ButtonState;

typedef struct GameInputs {
    ButtonState mouse1;
    ButtonState mouse2;
    ButtonState ctrl;
    ButtonState space;
} GameInputs;

void set_button_state(ButtonState* button);
