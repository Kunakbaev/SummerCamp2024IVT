#ifndef _COLOURFULL_PRINT_HEADER_
#define _COLOURFULL_PRINT_HEADER_

#ifndef printf
    #include <stdio.h>
#endif

// bold:
// char* const RED_COLOR[2] = {"\033[1;31m", "\033[0m"};


/// @brief handy Colors enum, user can only choose between them
enum Colors {
    RED_COLOR     = 0,
    GREEN_COLOR   = 1,
    YELLOW_COLOR  = 2,
    BLUE_COLOR    = 3,
    MAGENTA_COLOR = 4,
    CYAN_COLOR    = 5,
    WHITE_COLOR   = 6,
    BASE_COLOR    = WHITE_COLOR,
};

/**
    \brief returns string matching color
    \param[in] color Colors enum, that is transformed to string
    \return color in string representation
*/
const char* getColor(enum Colors color);


/**
    \brief changes text color
    Changes CURRENT_COLOR value to the new color
    \param[in] color string that represents color to which we switch
*/
void changeTextColor(enum Colors newColor);


/// @brief return current text color
Colors getCurrentColor();

/**
    \brief printf but with color equal to CURRENT_COLOR
    \param[in] ... printf args
*/
#define colourfullPrint(...)                             \
    do {                                                 \
        printf("%s", getColor(getCurrentColor()));   \
        printf(__VA_ARGS__);                             \
        printf("%s", getColor(BASE_COLOR));          \
    } while(0)

/// @brief prints in stderror but with red color
#define printError(...)                                  \
    do {                                                 \
        fprintf(stderr, "%s", getColor(RED_COLOR));  \
        fprintf(stderr, __VA_ARGS__);                    \
        fprintf(stderr, "%s", getColor(BASE_COLOR)); \
    } while(0)

#endif


