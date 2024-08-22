#include "colourfullPrint.hpp"

void changeTextColor(enum Colors newColor) {
    CURRENT_COLOR = newColor;
    printf("CURRENT COLOR: %d\n", CURRENT_COLOR);
    printf("color: %s fakdsfj;l\n", colorStrings[CURRENT_COLOR]);
}
