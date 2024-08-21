/*!

    \brief class main file for testing

*/

#include <stdio.h>
#include "testsGeneratorLib/testsGenerator.hpp"


// ----------------------------- MAIN ----------------------------------------
int main() {
    // checking if solution works on custsom tests
    printf("Running on tests: \n");

    Tester tester; // init
    validateAllTests(&tester);
    tester.GetSolutionsFunc = &getSolutions;
    CheckOnTestsOutput result = checkOnTests(&tester);

    return result.state;

    return 0;
}
