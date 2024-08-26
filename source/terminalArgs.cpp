/**
    \file
    \brief this file contains realizations of methods that work with arguments from terminal
*/

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/terminalArgs.hpp"
#include "../LoggerLib/include/colourfullPrint.hpp"
#include "../LoggerLib/include/logLib.hpp"

/// @brief error occures if output file is not specified
const char* const FILE_ARGUMENTS_ERROR       = "Error: file arguments are invalid\n";

/// @brief error occures if user input is not provided
const char* const USER_INPUT_ARGUMENTS_ERROR = "Error: user input arguments are invalid\n";

/// @brief error occures if provided argument are invalid long doubles
const char* const INCORRECT_USER_INPUT_ERROR = "Error: incorrect user input\n";

/// @brief error occures if some of arguments flags are unknow
const char* const UNKNOWN_FLAGS_ERROR        = "Error: there are some unknown flags\n";

/// @brief error occures if memory is not allocated during calloc or malloc
const char* const MEMORY_ALLOCATION_ERROR    = "Error: couldn't allocate memory\n";

const char* USER_FLAG_SHORT      = "-u";
const char* USER_FLAG_EXTENDED   = "--user";
const char* HELP_FLAG_SHORT      = "-h";
const char* HELP_FLAG_EXTENDED   = "--help";
const char* OUTPUT_FLAG_SHORT    = "-o";
const char* OUTPUT_FLAG_EXTENDED = "--output";
const char* TESTS_FLAG_SHORT    = "-t";
const char* TESTS_FLAG_EXTENDED = "--test";

static bool isKnownFlag(const char* flag) {
    const char* const arr[] = {
        USER_FLAG_SHORT,
        USER_FLAG_EXTENDED,
        HELP_FLAG_SHORT,
        HELP_FLAG_EXTENDED,
        OUTPUT_FLAG_SHORT,
        OUTPUT_FLAG_EXTENDED,
        TESTS_FLAG_SHORT,
        TESTS_FLAG_EXTENDED,
    };

    int arrLen = sizeof(arr) / sizeof(*arr);
    for (int i = 0; i < arrLen; ++i)
        if (strcmp(arr[i], flag) == 0)
            return true;
    return false;
}

static bool isParamFlag(const char* arg) {
    assert(arg != NULL);
    if (arg[0] != '-')
        return false;
    int cntBlanks = 0;
    for (size_t i = 0; i < strlen(arg); ++i)
        cntBlanks += isblank(arg[i]);
    return cntBlanks == 0;
}

void validateManager(const ArgsManager* manager) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    assert(manager       != NULL);
    assert(manager->argv != NULL);


    for (int i = 1; i < manager->argc; ++i) {
        const char* flag = manager->argv[i];
        if (isParamFlag(flag) && !isKnownFlag(flag)) {
            LOG_ERROR("%s", UNKNOWN_FLAGS_ERROR);
            printError("%s", UNKNOWN_FLAGS_ERROR);
            return;
        }
    }
}

static int findOneCommandIndexFromArgs(const ArgsManager* manager, const char* flag) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    ///\throw flag should not be NULL
    assert(manager       != NULL);
    assert(manager->argv != NULL);
    assert(flag != NULL);

    for (int i = 1; i < manager->argc; ++i)
        if (strcmp(manager->argv[i], flag) == 0)
            return i;

    // flag was not found in arguments to program
    return -1;
}

static int findCommandIndex(const ArgsManager* manager, const char* flag_short, const char* flag_extended) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    ///\throw flag_short should not be NULL
    ///\throw flag_extended should not be NULL
    assert(manager->argv != NULL);
    assert(manager->argv != NULL);
    assert(flag_short    != NULL);
    assert(flag_extended != NULL);

    // tries to get atleast one good index of flag
    int first  = findOneCommandIndexFromArgs(manager, flag_short);
    int second = findOneCommandIndexFromArgs(manager, flag_extended);
    // printf("first : %d, second : %d\n", first, second);
    return first > second ? first : second;
}

/**
    \brief checks that there are enough arguments to flag and they are valid (not other flags)
    \param[in] manager contains argc and argv
    \param[in] startInd index of flag in argv
    \param[in] cntNeed number of arguments to flag
*/
static bool checkGoodParams(const ArgsManager* manager, int startInd, int cntNeed) {
    assert(manager != NULL);
    assert(manager->argv != NULL);

    if (startInd + cntNeed >= manager->argc)
        return false;

    for (int i = 1; i <= cntNeed; ++i)
        if (isParamFlag(manager->argv[startInd + i]))
            return false;
    return true;
}

const char* parseOutputFile(const ArgsManager* manager) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    assert(manager       != NULL);
    assert(manager->argv != NULL);

    int ind = findCommandIndex(manager, OUTPUT_FLAG_SHORT, OUTPUT_FLAG_EXTENDED);
    // output file argument not found
    if (ind == -1) return NULL;

    const int numNeededArgs = 1;
    if (!checkGoodParams(manager, ind, numNeededArgs)) {
        LOG_ERROR("%s", FILE_ARGUMENTS_ERROR);
        printError("%s", FILE_ARGUMENTS_ERROR);
        return NULL;
    }

    return manager->argv[ind + 1];
}

// FIXME: this function is not very stable
bool parseUserInput(const ArgsManager* manager, QuadraticEquation* eq) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    ///\throw eq should not be NULL
    assert(manager       != NULL);
    assert(manager->argv != NULL);
    assert(eq != NULL);

    int ind = findCommandIndex(manager, USER_FLAG_SHORT, USER_FLAG_EXTENDED);
    // output file argument not found
    if (ind == -1) return false;

    const int numNeededArgs = 1;
    if (!checkGoodParams(manager, ind, numNeededArgs)) {
        LOG_ERROR("%s", USER_INPUT_ARGUMENTS_ERROR);
        printError("%s", USER_INPUT_ARGUMENTS_ERROR);
        return false;
    }

    char* line = (char*)calloc(strlen(manager->argv[ind + 1]) + 2, sizeof(*manager->argv[ind + 1]));
    if (line == NULL)
        LOG_ERROR("%s", MEMORY_ALLOCATION_ERROR);
    strcpy(line, manager->argv[ind + 1]);

    int cntBlanks = 0;
    for (int i = 0; i < (int)strlen(line); ++i)
        cntBlanks += isblank(line[i]);
    if (cntBlanks != 2) {
        LOG_ERROR("%s", USER_INPUT_ARGUMENTS_ERROR);
        printError("%s", USER_INPUT_ARGUMENTS_ERROR);
        return false;
    }
    strcat(line, " ");

    // 3 -> cnt of coefficient of quadratic equation
    int argInd = 0;
    long double* const arr[3] = {&eq->a, &eq->b, &eq->c};
    char* word = (char*)calloc(strlen(line) + 1, sizeof(*line));
    if (word == NULL)
        LOG_ERROR("%s", MEMORY_ALLOCATION_ERROR);
    eq->outputPrecision = DEFAULT_PRECISION; // global const STD_PRECISION
    // printf("word : %s, len : %d\n", word, strlen(word));

    for (int i = 0; line[i] != '\0'; ++i) {
        if (!isblank(line[i])) {
            size_t len = strlen(word);
            word[len] = line[i];
            word[len + 1] = '\0';
            continue;
        }

        bool isOk = false;
        QuadEqErrors error = parseLongDoubleAndCheckValid(word, arr[argInd], &isOk);

        if (error) {
            LOG_ERROR("%s", getErrorMessage(error));
            printError("%s", getErrorMessage(error));
            return false;
        }
        if (!isOk) {
            LOG_ERROR("%s", getErrorMessage(error));
            printError("%s", INCORRECT_USER_INPUT_ERROR);
            return false;
        }
        ++argInd;
        word[0] = '\0';
    }
    free(line);
    free(word);

    return true;
}

bool isHelpNeeded(const ArgsManager* manager) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    assert(manager != NULL);
    assert(manager->argv != NULL);

    int ind = findCommandIndex(manager, HELP_FLAG_SHORT, HELP_FLAG_EXTENDED);
    return ind != -1;
}

/// @brief
/// @param manager
/// @param isTest
/// @note The returning pointer is heap-allocated, do not forget to free!
/// @return
char* parseTestsArgs(const ArgsManager* manager, bool* isTest) {
    ///\throw manager should not be NULL
    ///\throw manager->argv should not be NULL
    assert(manager != NULL);
    assert(manager->argv != NULL);
    assert(isTest != NULL);

    char* outputFile = {};
    int ind = findCommandIndex(manager, TESTS_FLAG_SHORT, TESTS_FLAG_EXTENDED);
    if (ind == -1) {
        *isTest = false;
        return NULL;
    }

    *isTest = true;
    const int cntNeedArgs = 1;
    bool isGood = checkGoodParams(manager, ind, cntNeedArgs);
    if (isGood) {
        outputFile = (char*)calloc(strlen(manager->argv[ind + 1]) + 1, sizeof(*(manager->argv[ind + 1])));
        if (outputFile == NULL)
            LOG_ERROR("%s", MEMORY_ALLOCATION_ERROR);
        strcpy(outputFile, manager->argv[ind + 1]);
        return outputFile;
    }

    return NULL;
}
