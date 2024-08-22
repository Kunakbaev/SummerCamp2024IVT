#ifndef TERMINAL_ARGS_LIB
#define TERMINAL_ARGS_LIB

/**
    \file
    \brief this file contains methods that work with arguments from terminal
*/

#include "../quadraticEquationLib/quadraticEquation.hpp"

/// @brief prints helping message, for user to understand what this programm does
const char* const HELP_MESSAGE                  = "This app solves quadratic equation.\n"
                                                  "It works only with rational coefficients and finds rational solutions.\n"\
                                                  "Possible terminal arguments:\n"
                                                  "--help   (-h)          prints helping message (current command)\n"
                                                  "--user   (-u) a b c    specifies coefficients of equation via user input (a, b, c)\n"
                                                  "--output (-o)          specifies output file\n";

/// @brief error occures if output file is not specified
const char* const FILE_NOT_PROVIDED_ERROR       = "Error: file name not provided\n";

/// @brief error occures if user input is not provided
const char* const USER_INPUT_NOT_PROVIDED_ERROR = "Error: user input not provided\n";

/// @brief error occures if provided argument are invalid long doubles
const char* const INCORRECT_USER_INPUT_ERROR    = "Error: incorrect user input\n";



/**
    \brief parses outputFile name from terminal arguments
    \param[in] argc number of terminal arguments
    \param[in] argv terminal arguments
    \result outputFile name
*/
const char* parseOutputFile(int argc, char** argv);

/**
    \brief parses coefficients from terminal arguments
    \param[in] argc number of terminal arguments
    \param[in] argv terminal arguments
    \result was parse successful
*/
bool parseUserInput(int argc, char** argv, QuadraticEquation* eq);

/**
    \brief checks if help flag occurs in terminal arguments
    \param[in] argc number of terminal arguments
    \param[in] argv terminal arguments
    \result is help message needed
*/
bool isHelpNeeded(int argc, char** argv);

#endif
