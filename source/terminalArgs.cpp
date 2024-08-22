/**
    \file
    \brief this file contains realizations of methods that work with arguments from terminal
*/

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../include/terminalArgs.hpp"
#include "../include/colourfullPrint.hpp"

/// @brief error occures if output file is not specified
const char* const FILE_ARGUMENTS_ERROR       = "Error: file arguments are invalid\n";

/// @brief error occures if user input is not provided
const char* const USER_INPUT_ARGUMENTS_ERROR = "Error: user input arguments are invalid\n";

/// @brief error occures if provided argument are invalid long doubles
const char* const INCORRECT_USER_INPUT_ERROR = "Error: incorrect user input\n";

/// @brief error occures if some of arguments flags are unknow
const char* const UNKNOWN_FLAGS_ERROR        = "Error: there are some unknown flags\n";

const char* USER_FLAG_SHORT      = "-u";
const char* USER_FLAG_EXTENDED   = "--user";
const char* HELP_FLAG_SHORT      = "-h";
const char* HELP_FLAG_EXTENDED   = "--help";
const char* OUTPUT_FLAG_SHORT    = "-o";
const char* OUTPUT_FLAG_EXTENDED = "--output";

static bool isKnownFlag(const char* flag) {
    const char* const arr[] = {
        USER_FLAG_SHORT,
        USER_FLAG_EXTENDED,
        HELP_FLAG_SHORT,
        HELP_FLAG_EXTENDED,
        OUTPUT_FLAG_SHORT,
        OUTPUT_FLAG_EXTENDED
    };

    int arrLen = sizeof(arr) / sizeof(*arr);
    for (int i = 0; i < arrLen; ++i)
        if (strcmp(arr[i], flag) == 0)
            return true;
    return false;
}

static bool isParamArgument(const char* arg) {
    assert(arg != NULL);
    return arg[0] == '-';
}

void validateManager(const ArgsManager* manager) {
    ///\throw manager should not be NULL
    assert(manager != NULL);

    for (int i = 1; i < manager->argc; ++i) {
        const char* flag = manager->argv[i];
        if (isParamArgument(flag) && !isKnownFlag(flag)) {
            printError("%s", UNKNOWN_FLAGS_ERROR);
            return;
        }
    }
}

static int findOneCommandIndexFromArgs(const ArgsManager* manager, const char* flag) {
    ///\throw manager should not be NULL
    ///\throw argv should not be NULL
    ///\throw command should not be NULL
    assert(manager != NULL);
    assert(manager->argv != NULL);
    assert(flag != NULL);

    for (int i = 1; i < manager->argc; ++i)
        if (strcmp(manager->argv[i], flag) == 0)
            return i;

    // command was not found in arguments to program
    return -1;
}

static int findCommandIndex(const ArgsManager* manager, const char* flag_short, const char* flag_extended) {
    ///\throw manager should not be NULL
    ///\throw argv should not be NULL
    ///\throw flag_short should not be NULL
    ///\throw flag_extended should not be NULL
    assert(manager->argv != NULL);
    assert(flag_short != NULL);
    assert(flag_extended != NULL);

    // tries to get atleast one good index of flag
    int first  = findOneCommandIndexFromArgs(manager, flag_short);
    int second = findOneCommandIndexFromArgs(manager, flag_extended);
    // printf("first : %d, second : %d\n", first, second);
    return first > second ? first : second;
}

// FIXME: check for illegal flags

static bool checkGoodParams(const ArgsManager* manager, int startInd, int cntNeed) {
    assert(manager != NULL);
    assert(manager->argv != NULL);

    if (startInd + cntNeed >= manager->argc)
        return false;

    for (int i = 1; i <= cntNeed; ++i)
        if (isParamArgument(manager->argv[startInd + i]))
            return false;
    return true;
}

const char* parseOutputFile(const ArgsManager* manager) {
    ///\throw manager should not be NULL
    ///\throw argv should not be NULL
    assert(manager != NULL);
    assert(manager->argv != NULL);

    int ind = findCommandIndex(manager, OUTPUT_FLAG_SHORT, OUTPUT_FLAG_EXTENDED);
    // output file argument not found
    if (ind == -1) return NULL;

    const int numNeededArgs = 1;
    if (!checkGoodParams(manager, ind, numNeededArgs)) {
        printError("%s", FILE_ARGUMENTS_ERROR);
        return NULL;
    }

    return manager->argv[ind + 1];
}

bool parseUserInput(const ArgsManager* manager, QuadraticEquation* eq) {
    ///\throw manager should not be NULL
    ///\throw argv should not be NULL
    ///\throw eq should not be NULL
    assert(manager != NULL);
    assert(manager->argv != NULL);
    assert(eq != NULL);

    int ind = findCommandIndex(manager, USER_FLAG_SHORT, USER_FLAG_EXTENDED);
    // output file argument not found
    if (ind == -1) return false;

    const int numNeededArgs = 3;
    if (!checkGoodParams(manager, ind, numNeededArgs)) { // same
        printError("%s", USER_INPUT_ARGUMENTS_ERROR);
        return false;
    }

    eq->outputPrecision = DEFAULT_PRECISION; // global const STD_PRECISION
    //FIXME: array of pointers to structure fields???

    // 3 -> cnt of coefficient of quadratic equation
    long double* const arr[3] = {&eq->a, &eq->b, &eq->c};
    for (int i = 0; i < 3; ++i) {
        const char* word = manager->argv[ind + i + 1];
        bool isOk = false;
        char* argCopy = (char*)calloc(sizeof(*word), strlen(word));
        strcpy(argCopy, word);

        QuadEqErrors error = parseLongDoubleAndCheckValid(argCopy, arr[i], &isOk); // &arr[5] / arr + 5
        free(argCopy);

        if (error) {
            printError("%s", errorMessages[error]);
            return false;
        }
        if (!isOk) {
            printError("%s", INCORRECT_USER_INPUT_ERROR);
            return false;
        }
    }

    return true;
}

bool isHelpNeeded(const ArgsManager* manager) {
    ///\throw manager should not be NULL
    ///\throw argv should not be NULL
    assert(manager != NULL);
    assert(manager->argv != NULL);

    int ind = findCommandIndex(manager, HELP_FLAG_SHORT, HELP_FLAG_EXTENDED);
    return ind != -1;
}
