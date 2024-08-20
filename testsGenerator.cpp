#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "testsGenerator.hpp" // FIXME: pochitat' ob posledovatelnosti include


const char* TOO_FEW_TESTS_ERROR = "Error: there are too few tests\n";

static int getCntOfTests(const Tester* tester) {
    assert(tester != NULL && tester->tests != NULL);
    return sizeof(tester->tests) / sizeof(*tester->tests);
}

static bool checkIfAnswerEqual(const QuadraticEquationAnswer* one, const QuadraticEquationAnswer* two) {
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

CheckOnTestsOutput checkOnTests(const Tester* tester, getSolutionsFuncPtr GetSolutionsFunc) {
    assert(tester != NULL && tester->tests != NULL);

    CheckOnTestsOutput result;
    int arrLen = getCntOfTests(tester);
    for (int i = 0; i < arrLen; ++i) {
        QuadraticEquationAnswer answer;
        (*GetSolutionsFunc)(&tester->tests[i].equation, &answer);
        if (!checkIfAnswerEqual(&answer, &(tester->tests[i].answer))) {
            printSolutions(&answer, 10);
            printSolutions(&tester->tests[i].answer, 10);
            result.testIndex = i;
            result.state = FAILED_ON_SOME_TEST;
            return result;
        }
    }

    result.state = ALL_TESTS_PASSED;
    return result;
}

void printTest(const Tester* tester, const Test* test) {
    assert(tester != NULL && tester->tests != NULL);

    printf("-------------------------\n");
    printEquation(&test->equation);
    printSolutions(&test->answer, test->equation.outputPrecision);
}

void printAllTests(const Tester* tester) {
    assert(tester != NULL && tester->tests != NULL);

    int arrLen = getCntOfTests(tester);
    printf("All tests:\n");
    for (int i = 0; i < arrLen; ++i)
        printTest(tester, &tester->tests[i]);
}

void printTestWithInd(const Tester* tester, int testIndex) {
    assert(tester != NULL && tester->tests != NULL);

    if (testIndex >= getCntOfTests(tester)) {
        fprintf(stderr, "%s", TOO_FEW_TESTS_ERROR);
        return;
    }

    printTest(tester, &tester->tests[testIndex]);
}




