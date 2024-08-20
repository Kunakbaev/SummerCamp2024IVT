/*!

    \brief class usecase file
    In this file all methods of class are used to show how QuadraticEquation class works.

*/

#include <stdio.h>

#define RUN_ON_TESTS

#ifdef RUN_ON_TESTS
#include "testsGenerator.hpp"
#else
#include "quadraticEquationLib.hpp"
#endif


// ----------------------------- MAIN ----------------------------------------

int main() {
#ifdef RUN_ON_TESTS
    printf("Running on tests: \n");
    QuadraticEquationAnswer answer;

    Tester tester;
    CheckOnTestsOutput result = checkOnTests(&tester, &getSolutions);
    if (result.state == FAILED_ON_SOME_TEST) {
        printf("Failed on test: %d\n", result.testIndex);
    } else {
        printf("All tests passed\n");
    }
#else
    // usecase of QuadraticEquation class
    struct QuadraticEquation equation;

    readEquation(&equation);
    printEquation(&equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise error will occur
    printf("Coordinate X of top of parabola: %.10Lg\n", getVertX(&equation));
    printf("Coordinate Y of top of parabola: %.10Lg\n", getVertY(&equation));
    printf("Value at point 5: %.10Lg\n", getPointValue(&equation, 5));
    solveAndPrintEquation(&equation);
#endif

    return 0;
}
