/*!

    \brief class usecase file
    In this file all methods of class are used to show how QuadraticEquation class works.

*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "../LoggerLib/colourfulPrintLib/colourfullPrint.h"
#include "../include/testsGenerator.hpp"
#include "../include/quadraticEquation.hpp"
#include "../include/terminalArgs.hpp"

//extern "C" {
    #include "../LoggerLib/logLib.h"
//}


// ----------------------------- MAIN ----------------------------------------


void quadraticEquationShowcase(struct QuadraticEquation* equation, const char* outputFile);
int runOnTests(char* testsFileSource);

int main(int argc, const char* const argv[]) {
#ifdef RUN_ON_TESTS
    return runOnTests(NULL);
#endif

    // ----------------------     LOGGER INIT      ----------------------------------

    setLoggingLevel(DEBUG);
    stateLogFile("../loggingFile.txt");
    LOG_DEBUG("i am debug\n");
    LOG_INFO("i am info\n");
    LOG_WARNING("i am warning\n");
    LOG_ERROR("i am error\n");

    // DEBUG("fdas");





    // -----------------------------    GETING ARGS FROM CONSOLE     ----------------------------------

    // for (int i = 0; i < argc; ++i) {
    //     printf("%s\n", argv[i]);
    // }

    struct QuadraticEquation equation = {};

    ArgsManager manager = {argc, argv};
    validateManager(&manager);

    if (isHelpNeeded(&manager)) {
        changeTextColor(YELLOW_COLOR);
        colourfullPrint("%s", HELP_MESSAGE);
        return 0;
    }

    const char* outputFile = parseOutputFile(&manager);

    bool isTestRun = false;
    char* testsFileSource = parseTestsArgs(&manager, &isTestRun);
    //printf("isTest : %d, TestSource : %s\n", isTestRun, testsFileSource);
    if (isTestRun) {
        int code = runOnTests(testsFileSource);
        free(testsFileSource);
        testsFileSource = NULL;

        destructLogger();
        return code;
    }
    free(testsFileSource);
    testsFileSource = NULL;

    if (!parseUserInput(&manager, &equation))
        readEquation(&equation);
    quadraticEquationShowcase(&equation, outputFile);

    destructLogger();
    return 0;
}

// usecase of QuadraticEquation class
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
    free(tester.membuffer);
    tester.membuffer = NULL;

    return result.state;
}
