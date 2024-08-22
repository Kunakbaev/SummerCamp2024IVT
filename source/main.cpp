/*!

    \brief class usecase file
    In this file all methods of class are used to show how QuadraticEquation class works.

*/

#include <stdio.h>
#include <string.h>
#include "../include/colourfullPrint.hpp"

//#define RUN_ON_TESTS

#ifdef RUN_ON_TESTS
#include "../include/testsGenerator.hpp"
#else
#include "../include/quadraticEquation.hpp"
#include "../include/terminalArgs.hpp"
#endif



// -u user input
// -o specify output file
// -h helping hint
// --help
// --output
// --user


// ----------------------------- MAIN ----------------------------------------
// ANSII colors



int main(int argc, char** argv) {
#ifdef RUN_ON_TESTS
// checking if solution works on custsom tests
    printf("Running on tests: \n");

    Tester tester; // init
    validateAllTests(&tester);
    tester.GetSolutionsFunc = &getSolutions;
    CheckOnTestsOutput result = checkOnTests(&tester);

    return result.state;
#else
    // changeTextColor(BLUE_COLOR);
    // colourfullPrint("fdsakl\n");
    // colourfullPrint("Hello world: %d %d\n", 10, 20);
    // changeTextColor(YELLOW_COLOR);
    // colourfullPrint("Hello world: %d %d %s\n", 10, 20, " I am green");


    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

    // usecase of QuadraticEquation class
    struct QuadraticEquation equation;

    ArgsManager manager = {argc, argv};
    validateManager(&manager);
    const char* outputFile = parseOutputFile(&manager);
    //printf("outpuFtile : %s\n", outputFile);

    if (isHelpNeeded(&manager)) {
        changeTextColor(YELLOW_COLOR);
        colourfullPrint("%s", HELP_MESSAGE);
        return 0;
    }

    QuadEqErrors error = QUAD_EQ_NO_ERROR;
    if (!parseUserInput(&manager, &equation)) {
        error = readEquation(&equation);
        if (error) {
            printError("%s", errorMessages[error]);
        }
    }



    // FIXME: intentional error FAFADAAAAAAAAAAAAAAAHHHHHHHHHHH
    error = printEquation(&equation);
    if (error) {
        printError("%s", errorMessages[error]);
    }

    // these two functions works only if it's quadratic equation (a != 0), otherwise error will occur
    long double vertX = 0, vertY = 0, pointValue = 0;
    error = getVertX(&equation, &vertX);
    if (error) {
        printError("%s", errorMessages[error]);
    }
    printf("Coordinate X of top of parabola: %.10Lg\n", vertX);

    error = getVertY(&equation, &vertY);
    if (error) {
        printError("%s", errorMessages[error]);
    }
    printf("Coordinate Y of top of parabola: %.10Lg\n", vertY);

    error = getPointValue(&equation, 5, &pointValue);
    if (error) {
        printError("%s", errorMessages[error]);
    }
    printf("Value at point 5: %.10Lg\n", pointValue);

    error = solveAndPrintEquation(&equation, outputFile);
    if (error) {
        printError("%s", errorMessages[error]);
    }
#endif

    return 0;
}
