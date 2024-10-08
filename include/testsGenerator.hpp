#ifndef TESTS_GENERATOR_HEADER
#define TESTS_GENERATOR_HEADER


/**
    \file
    \brief class containing all methods from testsGenerator lib
    In this file prototypes of all methods of class testsGenerator are declared
*/

#include "quadraticEquation.hpp"


// enum testsGeneratorErrors {
//     TESTS_GENERATOR_NO_ERROR              = 0,
//     TESTS_GENERATOR_ILLEGAL_ARG           = 1,
//     TESTS_GENERATOR_TOO_FEW_TESTS_ERROR   = 2,
//     TESTS_GENERATOR_VALIDATION_FAIL_ERROR = 3,
// };
//
// const char* const errorMessages[] = {
//     "No errors occured\n",
//     "Error: illegal argument (possibly set to NULL)\n",
//     "Error: file arguments are invalid\n",
//     "Error: user input arguments are invalid\n",
//     "Error: incorrect user input\n"
// };




/**
    \brief Test, contains equation and answer

    Structure contains Test: equation and answer (solutions and equation state)
*/


struct Test {
    QuadraticEquation equation; ///< Test's equation
    QuadraticEquationAnswer answer; ///< Answer to test
};

/// @brief Testing state (failed or not)
enum CheckOnTestsState {
    ALL_TESTS_PASSED    = 0, ///< All tests passed, solution works on them correctly
    FAILED_ON_SOME_TEST = 1, ///< Solution has failed on some test
};

/**
    \brief Contains result of testing and if failed, number of test
    If solution has failed -> it happend on test with index testIndex (int 0-indexation)
*/
struct CheckOnTestsOutput {
    int testIndex; ///< test on which solution failed
    CheckOnTestsState state; ///< testing state
};

/**
    handy pointer to a solver function
    \code
    typedef void (*getSolutionsFuncPtr)(const struct QuadraticEquation*, struct QuadraticEquationAnswer*);
    \endcode
*/
typedef QuadEqErrors (*getSolutionsFuncPtr)(const struct QuadraticEquation*, struct QuadraticEquationAnswer*);



/**
    \brief Tester class, core of a lib

    Contains tests, can print them, check solution on tests
*/
struct Tester {
    int cntOfTests; ///< number of tests we have
    /**
        \brief all tests
        \warning if there are 2 solutions, they should be place in assending order
    */

    void* membuffer;
    const struct Test* tests;
    /// pointer to a solver function
    getSolutionsFuncPtr GetSolutionsFunc; ///< \warning should not be NULL
};

/**
    \brief returns myTests variable
*/
const Test* getMyTests(Tester* tester);

/**
    \brief checks solution on all tests

    Runs given solution on all tests
    \param[in] GetSolutionsFunc pointer to function that solves quadratic equation
    \result testing results, test on which solution failed
    \memberof Tester
*/
CheckOnTestsOutput checkOnTests(const Tester* tester);

///\brief prints all tests
void printAllTests(const Tester* tester);
/**
    \brief prints given test
    \param[in] test pointer to Test that will be printed
    \memberof Tester
*/
void printTest(const Tester* tester, const struct Test* test);

/**
    \brief prints test at given index
    \param[in] testIndex given index of test
    \memberof Tester
*/
void printTestWithInd(const Tester* tester, int testIndex);

/**
    \brief checks if all tests are valid
    \param[in] testsFileSource reads from file if it's not NULL
    \param[in] tester that contains tests
    \memberof Tester
*/
void validateTester(Tester* tester, const char* testsFileSource);

#endif
