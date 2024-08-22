#ifndef TERMINAL_ARGS_LIB
#define TERMINAL_ARGS_LIB

/**
    \file
    \brief this file contains methods that work with arguments from terminal
*/

#include "quadraticEquation.hpp"

// enum terminalArgsErros {
//     TERMINAL_ARGS_NO_ERROR =                   0,
//     TERMINAL_ARGS_QUAD_EQ_ILLEGAL_ARG =        1,
//     TERMINAL_ARGS_FILE_ARGUMENTS_ERROR =       2,
//     TERMINAL_ARGS_USER_INPUT_ARGUMENTS_ERROR = 3,
//     TERMINAL_ARGS_INCORRECT_USER_INPUT_ERROR = 4
// };
//
// const char* const errorMessages[] = {
//     "No errors occured\n",
//     "Error: illegal argument (possibly set to NULL)\n",
//     "Error: file arguments are invalid\n",
//     "Error: user input arguments are invalid\n",
//     "Error: incorrect user input\n"
// };


/// @brief prints helping message, for user to understand what this programm does
const char* const HELP_MESSAGE = "This app solves quadratic equation.\n"
                                 "It works only with rational coefficients and finds rational solutions.\n"
                                 "Possible terminal arguments:\n"
                                 "--help   (-h)          prints helping message (current command)\n"
                                 "--user   (-u) a b c    specifies coefficients of equation via user input (a, b, c)\n"
                                 "--output (-o)          specifies output file\n";

struct ArgsManager {
    int argc;
    const char* const *argv;
};

/**
    \brief checks that there aren't any invalid flags
    \param[in] manager Manager that contains argc and argv
    \memberof ArgsManager
*/
void validateManager(const ArgsManager* manager);

/**
    \brief parses outputFile name from terminal arguments
    \param[in] manager Manager that contains argc and argv
    \result outputFile name
    \memberof ArgsManager
*/
const char* parseOutputFile(const ArgsManager* manager);

/**
    \brief parses coefficients from terminal arguments
    \param[in] manager Manager that contains argc and argv
    \result was parse successful
    \memberof ArgsManager
*/
bool parseUserInput(const ArgsManager* manager, QuadraticEquation* eq);

/**
    \brief checks if help flag occurs in terminal arguments
    \param[in] manager Manager that contains argc and argv
    \result is help message needed
    \memberof ArgsManager
*/
bool isHelpNeeded(const ArgsManager* manager);

#endif
