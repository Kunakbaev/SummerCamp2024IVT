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

#include "testsGenerator.hpp" // FIXME: pochitat' ob posledovatelnosti include

/// @brief error occurs if there are too few tests and testIndex is bigger than number of tests
const char* TOO_FEW_TESTS_ERROR = "Error: there are too few tests\n";

/// @brief error occurs if some tests are in invalid format
const char* VALIDATION_FAIL_ERROR = "Error: tests validation failed\n";

/**
    \brief returns number of tests
    \param[in] tester tester that contains tests
    \result number of tests
*/
static int getCntOfTests(const Tester* tester) {
    ///\throw \param[in] tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);
    return sizeof(tester->tests) / sizeof(*tester->tests);
}

/**
    \brief return if answers are equal
    \param[in] one, two answers from getSolutions func and tests
    \result are two answer equal
*/
static bool checkIfAnswerEqual(const QuadraticEquationAnswer* one, const QuadraticEquationAnswer* two) {
    ///\throw one should not be NULL
    ///\throw two should not be NULL
    assert(one != NULL && two != NULL);
    if (one->numOfSols != two->numOfSols)
        return false;
    if (one->numOfSols == INFINITE_ROOTS)
        return true;

    if (one->numOfSols >= ONE_ROOT && sign(one->root_1 - two->root_1) != 0)
        return false;
    if (one->numOfSols == TWO_ROOTS && sign(one->root_2 - two->root_2) != 0)
        return false;
    return true;
}

/**
    \brief runs given getSolutions func on all tests from tester
    \param[in] tester tester that contains tests and pointer to solver func
    \result returns testing state and test index
*/
CheckOnTestsOutput checkOnTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    CheckOnTestsOutput result;
    int arrLen = getCntOfTests(tester);
    for (int i = 0; i < arrLen; ++i) {
        QuadraticEquationAnswer answer;
        (*tester->GetSolutionsFunc)(&tester->tests[i].equation, &answer);
        if (!checkIfAnswerEqual(&answer, &(tester->tests[i].answer))) {
            printf("Failed on test: #%d\n", i);
            printf("Test (expected):\n");
            printTest(tester, &tester->tests[i]);
            printf("Yours (wrong):\n");
            printSolutions(&answer, 10);
            result.testIndex = i;
            result.state = FAILED_ON_SOME_TEST;
            return result;
        }
    }

    printf("All tests passed\n");
    result.state = ALL_TESTS_PASSED;
    return result;
}

/**
    \brief prints given test
    \param[in] tester that contains tests
*/
void printTest(const Tester* tester, const Test* test) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    printf("-------------------------\n");
    printEquation(&test->equation);
    printSolutions(&test->answer, test->equation.outputPrecision);
}

/**
    \brief prints all tests
    \param[in] tester that contains tests
*/
void printAllTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    int arrLen = getCntOfTests(tester);
    printf("All tests:\n");
    for (int i = 0; i < arrLen; ++i)
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

    if (testIndex >= getCntOfTests(tester)) {
        fprintf(stderr, "%s", TOO_FEW_TESTS_ERROR);
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

    if (test->answer.numOfSols == TWO_ROOTS &&
        sign(test->answer.root_1 - test->answer.root_2) >= 0)
            return false;

    if (test->answer.numOfSols >= ONE_ROOT) {
        long double val = getPointValue(&test->equation, test->answer.root_1);
        if (sign(val)) return false;
    }
    if (test->answer.numOfSols == TWO_ROOTS) {
        long double val = getPointValue(&test->equation, test->answer.root_2);
        if (sign(val)) return false;
    }

    return true;
}

void validateAllTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    int arrLen = getCntOfTests(tester);
    for (int i = 0; i < arrLen; ++i) {
        Test test = tester->tests[i];
        if (!isValidTest(&test)) {
            printf("Test: %d\n", i);
            fprintf(stderr, "%s", VALIDATION_FAIL_ERROR);
            return;
        }
    }

    printf("All tests are valid\n");
}




