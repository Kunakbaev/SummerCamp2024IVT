#ifndef TERMINAL_ARGS_LIB
#define TERMINAL_ARGS_LIB

/**
    \file
    \brief this file contains methods that work with arguments from terminal
*/

#include "../quadraticEquationLib/quadraticEquation.hpp"

/// @brief prints helping message, for user to understand what this programm does
const char* const HELP_MESSAGE = "This app solves quadratic equation.\n"
                                 "It works only with rational coefficients and finds rational solutions.\n"
                                 "Possible terminal arguments:\n"
                                 "--help   (-h)          prints helping message (current command)\n"
                                 "--user   (-u) a b c    specifies coefficients of equation via user input (a, b, c)\n"
                                 "--output (-o)          specifies output file\n";

struct ArgsManager {
    int argc;
    char** argv;
};

/**
    \brief parses outputFile name from terminal arguments
    \param[in] argc number of terminal arguments
    \param[in] argv terminal arguments
    \result outputFile name
    \memberof ArgsManager
*/
const char* parseOutputFile(const ArgsManager* manager);

/**
    \brief parses coefficients from terminal arguments
    \param[in] argc number of terminal arguments
    \param[in] argv terminal arguments
    \result was parse successful
    \memberof ArgsManager
*/
bool parseUserInput(const ArgsManager* manager, QuadraticEquation* eq);

/**
    \brief checks if help flag occurs in terminal arguments
    \param[in] argc number of terminal arguments
    \param[in] argv terminal arguments
    \result is help message needed
    \memberof ArgsManager
*/
bool isHelpNeeded(const ArgsManager* manager);

#endif
