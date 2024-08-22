/**
    \file
    \brief this file contains realizations of methods that work with arguments from terminal
*/

#include <string.h>
#include <stdio.h>
#include <cassert>

#include "terminalArgs.hpp"
#include "../colourfullPrintLib/colourfullPrint.hpp"





static int findOneCommandIndexFromArgs(int argc, char** argv, const char* command) {
    ///\throw argv should not be NULL
    ///\throw command should not be NULL
    assert(argv != NULL && command != NULL);

    for (int i = 0; i < argc; ++i)
        if (strcmp(argv[i], command) == 0)
            return i;

    // command was not found in arguments to program
    return -1;
}

static int findCommandIndex(int argc, char** argv, const char* command_1, const char* command_2) {
    ///\throw argv should not be NULL
    ///\throw command_1 should not be NULL
    ///\throw command_2 should not be NULL
    assert(argv != NULL && command_1 != NULL && command_2 != NULL);

    int first  = findOneCommandIndexFromArgs(argc, argv, command_1);
    int second = findOneCommandIndexFromArgs(argc, argv, command_2);
    // printf("first : %d, second : %d\n", first, second);
    return first > second ? first : second;
}

const char* parseOutputFile(int argc, char** argv) {
    ///\throw argv should not be NULL
    assert(argv != NULL);

    int ind = findCommandIndex(argc, argv, "-o", "--output");
    // output file argument not found
    if (ind == -1) return "";

    if (ind + 1 >= argc) {
        printError("%s", FILE_NOT_PROVIDED_ERROR);
        return "";
    }

    // char* res = NULL;
    // strcpy(res, argv[ind + 1]);
    // return res;
    return argv[ind + 1];
}

bool parseUserInput(int argc, char** argv, QuadraticEquation* eq) {
    ///\throw argv should not be NULL
    ///\throw eq should not be NULL
    assert(argv != NULL && eq != NULL);

    int ind = findCommandIndex(argc, argv, "-u", "--user");
    // output file argument not found
    if (ind == -1) return false;

    if (ind + 3 >= argc) {
        printError("%s", USER_INPUT_NOT_PROVIDED_ERROR);
        return false;
    }

    eq->outputPrecision = 10;

    // char* aS = NULL, *bS = NULL, *cS = NULL;
    // strcpy(aS, argv[ind + 1]);
    // strcpy(bS, argv[ind + 2]);
    // strcpy(cS, argv[ind + 3]);

    // bool isOk = parseLongDoubleAndCheckValid(aS, &eq->a) &&
    //             parseLongDoubleAndCheckValid(bS, &eq->b) &&
    //             parseLongDoubleAndCheckValid(cS, &eq->c);

    bool isOk = parseLongDoubleAndCheckValid(argv[ind + 1], &eq->a) &&
                parseLongDoubleAndCheckValid(argv[ind + 2], &eq->b) &&
                parseLongDoubleAndCheckValid(argv[ind + 3], &eq->c);
    if (!isOk) {
        printError("%s", INCORRECT_USER_INPUT_ERROR);
        return false;
    }

    return true;
}

bool isHelpNeeded(int argc, char** argv) {
    ///\throw argv should not be NULL
    assert(argv != NULL);

    int ind = findCommandIndex(argc, argv, "-h", "--help");
    return ind != -1;
}
