/*!

    \brief class usecase file
    In this file all methods of class are used to show how QuadraticEquation class works.

*/

#include <stdio.h>
#include "colourfullPrintLib/colourfullPrint.hpp"

#ifdef RUN_ON_TESTS
#include "testsGeneratorLib/testsGenerator.hpp"
#else
#include "quadraticEquationLib/quadraticEquation.hpp"
#endif


// ----------------------------- MAIN ----------------------------------------
// ANSII colors
int main() {

    changeTextColor(BLUE_COLOR);
    colourfullPrint("fdsakl\n");
    colourfullPrint("Hello world: %d %d\n", 10, 20);
    changeTextColor(YELLOW_COLOR);
    colourfullPrint("Hello world: %d %d %s\n", 10, 20, " I am green");

    return 0;

#ifdef RUN_ON_TESTS
    // checking if solution works on custsom tests
    printf("Running on tests: \n");

    Tester tester; // init
    validateAllTests(&tester);
    tester.GetSolutionsFunc = &getSolutions;
    CheckOnTestsOutput result = checkOnTests(&tester);
#else
    //usecase of QuadraticEquation class
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
