#include "colourfullPrint.hpp"

static enum Colors CURRENT_COLOR = RED_COLOR;

void changeTextColor(Colors newColor) {
    CURRENT_COLOR = newColor;
}

Colors getCurrentColor() {
    return CURRENT_COLOR;
}
