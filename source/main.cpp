/*!

    \brief class usecase file
    In this file all methods of class are used to show how QuadraticEquation class works.

*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/colourfullPrint.hpp"
#include "../include/testsGenerator.hpp"
#include "../include/quadraticEquation.hpp"
#include "../include/terminalArgs.hpp"



// -u user input
// -o specify output file
// -h helping hint
// --help
// --output
// --user


// ----------------------------- MAIN ----------------------------------------
// ANSII colors



void quadraticEquationShowcase(struct QuadraticEquation* equation, const char* outputFile);
int runOnTests(char* testsFileSource);



int main(int argc, const char* const argv[]) {
#ifdef RUN_ON_TESTS
    return runOnTests();
#endif

    // changeTextColor(BLUE_COLOR);
    // colourfullPrint("fdsakl\n");
    // colourfullPrint("Hello world: %d %d\n", 10, 20);
    // changeTextColor(YELLOW_COLOR);
    // colourfullPrint("Hello world: %d %d %s\n", 10, 20, " I am green");

    // for (int i = 0; i < argc; ++i) {
    //     printf("%s\n", argv[i]);
    // }

    // usecase of QuadraticEquation class
    struct QuadraticEquation equation = {};

    ArgsManager manager = {argc, argv};
    validateManager(&manager);

    if (isHelpNeeded(&manager)) {
        changeTextColor(YELLOW_COLOR);
        colourfullPrint("%s", HELP_MESSAGE);
        return 0;
    }

    const char* outputFile = parseOutputFile(&manager);
    //printf("outpuFtile : %s\n", outputFile);

    bool isTestRun = false;
    char* testsFileSource = parseTestsArgs(&manager, &isTestRun);
    //printf("isTest : %d, TestSource : %s\n", isTestRun, testsFileSource);
    if (isTestRun)
        return runOnTests(testsFileSource);

    if (!parseUserInput(&manager, &equation))
        readEquation(&equation);

    quadraticEquationShowcase(&equation, outputFile);

    return 0;
}

void quadraticEquationShowcase(struct QuadraticEquation* equation, const char* outputFile) {
    assert(equation != NULL);

    printEquation(equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise error will occur
    long double vertX = 0.0, vertY = 0.0, pointValue = 0.0;
    getVertX(equation, &vertX);
    printf("Coordinate X of top of parabola: %.10Lg\n", vertX);

    getVertY(equation, &vertY);
    printf("Coordinate Y of top of parabola: %.10Lg\n", vertY);

    getPointValue(equation, 5, &pointValue);
    printf("Value at point 5: %.10Lg\n", pointValue);

    solveAndPrintEquation(equation, outputFile);
}

int runOnTests(char* testsFileSource) {
    // checking if solution works on custsom tests
    printf("Running on tests: \n");

    Tester tester = {}; // init
    validateTester(&tester, testsFileSource);
    tester.GetSolutionsFunc = &getSolutions;
    CheckOnTestsOutput result = checkOnTests(&tester);

    return result.state;
}
