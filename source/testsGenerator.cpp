/**

    \file
    \brief class containing realization of all methods from testsGenerator lib

    This is main file, with all functionality of class. File contains realization of all methods of class Tester

*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "../LoggerLib/include/colourfullPrint.hpp"
#include "../LoggerLib/include/logLib.hpp"
#include "../include/quadraticEquation.hpp"
#include "../include/testsGenerator.hpp"

/// @brief error occurs if there are too few tests and testIndex is bigger than number of tests
const char* TOO_FEW_TESTS_ERROR           = "Error: there are too few tests\n";

/// @brief error occurs if some tests are in invalid format
const char* VALIDATION_FAIL_ERROR         = "Error: tests validation failed\n";

/// @brief error occurs if argument to function is set to NULL or it's reaches invalid case in switch
const char* ILLEGAL_ARG_ERROR             = "Error: invalid argument (possible set to NULL)\n";

/// @brief error occurs if argument to function is set to NULL or it's reaches invalid case in switch
const char* INVALID_FILE_ERROR            = "Error: invalid file name (or couldn't open it)\n";

/// @brief error occures if memory is not allocated during calloc or malloc
const char* const MEMORY_ALLOCATION_ERROR = "Error: couldn't allocate memory\n";

const int LINE_BUFFER_SIZE = 256;
const char BREAK_CHAR = '#';
const char* INF_ROOTS_IN_FILE = "inf\n";


//   a        b       c precision root_1           root_2           cnt_of_roots
const struct Test myTests[] = {
    {{1.0,    1.0,   -12.0, 10}, {-4,                3,               TWO_ROOTS}},
    {{1,     -1,      0.25, 10}, {0.5,               0.5,             ONE_ROOT}},
    {{0,     -3,      1,    10}, {1.0 / 3.0,         1.0 / 3.0,       ONE_ROOT}},
    {{0,      0,      1,    10}, {0,                 0,               NO_ROOTS}},
    {{0,      0,      0,    10}, {0,                 0,               INFINITE_ROOTS}},
    {{1,     -5,      9,    10}, {0,                 0,               NO_ROOTS}},
    {{1,     -4,      4,    10}, {2,                 2,               ONE_ROOT}},
    {{1,      3,     -4,    10}, {-4,                1,               TWO_ROOTS}},
    {{10,    -5,      9,    10}, {0,                 0,               NO_ROOTS}},
    {{0.581, -10.42, 0.592, 10}, {0.056994945705249, 17.877600579252, TWO_ROOTS}}
};


const Test* getMyTests(Tester* tester) {
    assert(tester != NULL);

    tester->cntOfTests = (int)(sizeof(myTests) / sizeof(*myTests));
    return myTests;
}


/**
    \brief returns number of tests
    \param[in] tester tester that contains tests
    \result number of tests
*/

/**
    \brief return if answers are equal
    \param[in] one, two answers from getSolutions func and tests
    \result are two answer equal
*/
                                        // FIXME: принято  vvv src                               ref
static bool checkIfAnswerEqual(const QuadraticEquationAnswer* mine, const QuadraticEquationAnswer* corr) {
    ///\throw mine should not be NULL
    ///\throw corr should not be NULL
    assert(mine != NULL);
    assert(corr != NULL);
    if (mine->numOfSols != corr->numOfSols)
        return false;

    switch (mine->numOfSols) {
        case NO_ROOTS:
            return true;
        case INFINITE_ROOTS:
            return true;
        case TWO_ROOTS:
            if (sign(corr->root_1 - corr->root_1) != 0)
                return false;
        case ONE_ROOT:
            return (sign(mine->root_2 - corr->root_2) == 0);
        default:
            assert(false);
            LOG_ERROR("%s", ILLEGAL_ARG_ERROR);
            break;
    }
    return false;
}

/**
    \brief runs given getSolutions func on all tests from tester
    \param[in] tester tester that contains tests and pointer to solver func
    \result returns testing state and test index
*/
CheckOnTestsOutput checkOnTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL);
    assert(tester->tests != NULL);

    CheckOnTestsOutput result = {};
    for (int i = 0; i < tester->cntOfTests; ++i) {
        QuadraticEquationAnswer answer = {};
        Test test = tester->tests[i];
        (*tester->GetSolutionsFunc)(&test.equation, &answer);
        if (!checkIfAnswerEqual(&answer, &test.answer)) {
            printf("Failed on test: #%d\n", i);
            printf("Test (expected):\n");
            printTest(tester, &test);
            printf("Yours (wrong):\n");
            printSolutions(&answer, DEFAULT_PRECISION, NULL);

            result.testIndex = i;
            result.state = FAILED_ON_SOME_TEST;
            return result;
        }
    }

    changeTextColor(GREEN_COLOR);
    colourfullPrint("All tests passed\n");
    result.state = ALL_TESTS_PASSED;
    return result;
}

/**
    \brief prints given test
    \param[in] tester that contains tests
*/
void printTest(const Tester* tester, const Test* test) {
    ///\throw tester should not be NULL
    assert(tester != NULL);

    printf("-------------------------\n");
    printEquation(&test->equation);
    printSolutions(&test->answer, test->equation.outputPrecision, NULL);
}

/**
    \brief prints all tests
    \param[in] tester that contains tests
*/
void printAllTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL);
    assert(tester->tests != NULL);

    printf("All tests:\n");
    for (int i = 0; i < tester->cntOfTests; ++i)
        printTest(tester, &tester->tests[i]);
}

/**
    \brief prints test with given index
    \param[in] tester that contains tests
    \param[in] testIndex index of test that will be outputed
*/
void printTestWithInd(const Tester* tester, int testIndex) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    if (testIndex >= tester->cntOfTests) {
        LOG_ERROR("%s", TOO_FEW_TESTS_ERROR);
        printError("%s", TOO_FEW_TESTS_ERROR);
        return;
    }
    printTest(tester, &tester->tests[testIndex]);
}

/**
    \brief checks if test is valid
    \param[in] test test to check
    \result is test valid
*/
static bool isValidTest(const Test* test) {
    ///\throw test should not be NULL
    assert(test != NULL);

    int numOfSols = test->answer.numOfSols;
    if (numOfSols == TWO_ROOTS &&
        sign(test->answer.root_1 - test->answer.root_2) >= 0)
            return false;

    long double val = NAN;
    QuadEqErrors error = {};
    switch (test->answer.numOfSols) {
        case NO_ROOTS:
            return true;

        case INFINITE_ROOTS:
            return true;

        case TWO_ROOTS:
            error = getPointValue(&test->equation, test->answer.root_1, &val);
            if (error) {
                LOG_ERROR("%s", getErrorMessage(error));
                printError("%s", getErrorMessage(error));
            }
            if (sign(val)) return false;
        case ONE_ROOT:
            error = getPointValue(&test->equation, test->answer.root_2, &val);
            if (error) {
                LOG_ERROR("%s", getErrorMessage(error));
                printError("%s", getErrorMessage(error));
            }
            if (sign(val)) return false;

            if (test->answer.numOfSols == ONE_ROOT &&
                sign(test->answer.root_1 - test->answer.root_2))
                return false;
            break;

        default:
            assert(false);
            LOG_ERROR("%s", ILLEGAL_ARG_ERROR);
            printError("%s", ILLEGAL_ARG_ERROR);
            break;
    }

    return true;
}

static bool isFileLineGood(const char* line) {
    const char* newLinePtr = strchr(line, '\n');
    if (newLinePtr == NULL) { // error
        return false;
    }

    newLinePtr = strchr(newLinePtr + 1, '\n');
    if (newLinePtr != NULL) // error
        return false;
    return true;
}

static int getCntOfTestsInSourceFile(FILE* source) {
    ///\throw testsFileSource should not be NULL
    assert(source != NULL);

    if (source == NULL) {
        LOG_ERROR("%s", INVALID_FILE_ERROR);
        printError("%s", INVALID_FILE_ERROR);
        return -1;
    }

    int cntOfTests = 0;
    char line[LINE_BUFFER_SIZE] = {};
    while (fgets(line, sizeof(line), source)) {
        if (!isFileLineGood(line))
            return -1;
        cntOfTests += line[0] == BREAK_CHAR;
    }

    return cntOfTests;
}

static void modifyCurrentTest(Test* currentTest, int varInd, long double number) {
    switch (varInd) {
        case 0: currentTest->equation.a    = number; break;
        case 1: currentTest->equation.b    = number; break;
        case 2: currentTest->equation.c    = number; break;
        case 3: currentTest->answer.root_1 = number; [[fallthrough]]
        case 4: currentTest->answer.root_2 = number; break;

        default:
            LOG_ERROR("%s", ILLEGAL_ARG_ERROR);
            printError("%s", ILLEGAL_ARG_ERROR);
            assert(false);
            break;
    }
}

static void chooseNumOfSols(QuadEqRootState* numOfSols, int varInd) {
    switch (varInd) {
        case 3: *numOfSols = NO_ROOTS;  break;
        case 4: *numOfSols = ONE_ROOT;  break;
        case 5: *numOfSols = TWO_ROOTS; break;

        default:
            LOG_ERROR("%s", ILLEGAL_ARG_ERROR);
            printError("%s", ILLEGAL_ARG_ERROR);
            assert(false);
            break;
    }
}

static void readTestsFromSourceFile(Tester* tester, FILE* source, int cntOfTests) {
    ///\throw testsFileSource should not be NULL
    ///\throw tests should not be NULL
    assert(source != NULL);
    assert(tester != NULL);
    assert(cntOfTests > 0);

    rewind(source);
    if (source == NULL) {
        LOG_ERROR("%s", INVALID_FILE_ERROR);
        printError("%s", INVALID_FILE_ERROR);
        return;
    }

    Test* testsCopy = (Test*)calloc((size_t)cntOfTests, sizeof(Test));
    if (testsCopy == NULL)
        LOG_ERROR("%s", MEMORY_ALLOCATION_ERROR);

    //tester->tests = testsCopy;

    bool isInf = false;
    int  varInd = 0;
    int testInd = 0;
    Test currentTest = {};
    char line[LINE_BUFFER_SIZE] = {};

    // if error, first we close file (source) and then return
    #define FAIL_AND_RETURN()       \
        do {                        \
            free(testsCopy);        \
            tester->tests = NULL;   \
            return;                 \
        } while(0);

    while (fgets(line, sizeof(line), source)) {
        if (!isFileLineGood(line))
            FAIL_AND_RETURN();

        if (line[0] == BREAK_CHAR) {
            if (!isInf)
                chooseNumOfSols(&currentTest.answer.numOfSols, varInd);
            else
                currentTest.answer.numOfSols = INFINITE_ROOTS;

            currentTest.equation.outputPrecision = DEFAULT_PRECISION;
            testsCopy[testInd++] = currentTest;
            varInd = 0;
            isInf = false;
            continue;
        }

        if (strcmp(line, INF_ROOTS_IN_FILE) == 0) {
            isInf = true;
            continue;
        }

        bool isOk = false;
        long double number = 0.0;
        parseLongDoubleAndCheckValid(line, &number, &isOk);
        if (!isOk)
            FAIL_AND_RETURN();

        modifyCurrentTest(&currentTest, varInd, number);
        ++varInd;
    }

    tester->cntOfTests = testInd;
    tester->membuffer = testsCopy;
    tester->tests = (const Test*)testsCopy;
}

static void readTests(Tester* tester, const char* testsFileSource) {
    ///\throw tester should not be NULL
    ///\throw testsFileSource should not be NULL
    assert(tester != NULL);
    assert(testsFileSource != NULL);

    FILE* source = fopen(testsFileSource, "r");
    if (source == NULL) {
        LOG_ERROR("%s", INVALID_FILE_ERROR);
        printError("%s", INVALID_FILE_ERROR);
        return;
    }

    // Пометка менторам: не давать пока что онегина, пусть пока так
    int cntOfTests = getCntOfTestsInSourceFile(source);
    if (cntOfTests == -1) { // error
        fclose(source);
        return;
    }

    readTestsFromSourceFile(tester, source, cntOfTests);
    fclose(source);
}

void validateTester(Tester* tester, const char* testsFileSource) {
    ///\throw tester should not be NULL
    assert(tester != NULL);

    tester->membuffer = NULL;
    if (testsFileSource == NULL)
        tester->tests = getMyTests(tester);
    else
        readTests(tester, testsFileSource);

    if (tester->tests == NULL) //error
        return;

    int arrLen = tester->cntOfTests;
    printAllTests(tester);

    // checking that tests are good
    for (int i = 0; i < arrLen; ++i) {
        const Test* test = (&tester->tests[i]);
        if (!isValidTest(test)) {
            printTest(tester, test);

            LOG_ERROR("Test: %d\n", i);
            LOG_ERROR("%s", VALIDATION_FAIL_ERROR);
            printError("Test: %d\n", i);
            printError("%s", VALIDATION_FAIL_ERROR);
            return;
        }
    }

    changeTextColor(GREEN_COLOR);
    colourfullPrint("All tests are valid\n");
}

/*
    LOG при входе в функцию
    LOG при выходе из функции
    LOG_ERROR   - логирование ошибки (вывод КРАСНЫМ!)
    LOG_WARNING - логирование варнинга (вывод ОРАНЖЕВЫМ!)
    LOG_INFO    - логирование информации (вывод белым)
    в stderr'ор

    максимальная ширина поля -
    время файл функция строка: Started функция

    Сделать дефайн, который отключает разные логи

    #define NO_LOG_INFO    - отключает инфу и функции
    #define NO_LOG_WARNING - отключает варнинги
    #define NO_LOG         - отключает все логи

    БОНУС: Если ты замечаешь, что файл перенаправлен, то отключаешь цветной вывод
*/
