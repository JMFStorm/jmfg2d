#include "j_input.h"

void set_button_state(ButtonState* button)
{
    int is_pressed = GetAsyncKeyState(button->key) & 0x8000;
    button->pressed = !button->is_down && is_pressed;
    button->is_down = is_pressed;
}
