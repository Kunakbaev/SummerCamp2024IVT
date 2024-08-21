#ifndef Colors

#ifndef printf
    #include <stdio.h>
#endif

// bold:
// char* const RED_COLOR[2] = {"\033[1;31m", "\033[0m"};

/// @brief handy Colors enum, user can only choose between them
enum Colors {
    RED_COLOR,
    GREEN_COLOR,
    YELLOW_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR,
    CYAN_COLOR,
    WHITE_COLOR,
    BASE_COLOR=WHITE_COLOR,
};

enum Colors CURRENT_COLOR = RED_COLOR;

/// @brief real values of each color
static char* const colorStrings[] = {
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[37m",
};


/**
    \brief changes text color
    Changes CURRENT_COLOR value to the new color
    \param[in] color string that represents color to which we switch
*/
void changeTextColor(enum Colors newColor) {
    CURRENT_COLOR = newColor;
}

/**
    \brief printf but with color equal to CURRENT_COLOR
    \param[in] ... printf args
*/
#define colourfullPrint(...) printf("%s", colorStrings[CURRENT_COLOR]); \
                            printf(__VA_ARGS__); \
                            printf("%s", colorStrings[BASE_COLOR]);

/// @brief prints in stderror but with red color
#define printError(...) fprintf(stderr, "%s", colorStrings[RED_COLOR]); \
                            fprintf(stderr, __VA_ARGS__); \
                            fprintf(stderr, "%s", colorStrings[BASE_COLOR]);

#endif


