/**
    \file
    \brief class containing all methods from testsGenerator lib
    In this file prototypes of all methods of class testsGenerator are declared
*/

#include "../quadraticEquationLib/quadraticEquation.hpp"

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
    ALL_TESTS_PASSED, ///< All tests passed, solution works on them correctly
    FAILED_ON_SOME_TEST, ///< Solution has failed on some test
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
typedef void (*getSolutionsFuncPtr)(const struct QuadraticEquation*, struct QuadraticEquationAnswer*);



/**
    \brief Tester class, core of a lib

    Contains tests, can print them, check solution on tests
*/
struct Tester {
    /**
        \brief all tests
        \warning if there are 2 solutions, they should be place in assending order
    */
    const struct Test tests[9] = {
        {{1.0, 1.0, -12.0, 10}, {-4, 3, TWO_ROOTS}},
        {{1,  -1,    0.35, 10}, {0.5, 0, ONE_ROOT}},
        {{0,  -3,    1,    10}, {1.0 / 3.0, 0, ONE_ROOT}},
        {{0,   0,    1,    10}, {0, 0, NO_ROOTS}},
        {{0,   0,    0,    10}, {0, 0, INFINITE_ROOTS}},
        {{1,  -5,    9,    10}, {0, 0, NO_ROOTS}},
        {{1,  -4,    4,    10}, {2, 0, ONE_ROOT}},
        {{1,   3,   -4,    10}, {-4, 1, TWO_ROOTS}},
        {{10, -5,    9,    10}, {0, 0, NO_ROOTS}}
    };
    /// pointer to a solver function
    getSolutionsFuncPtr GetSolutionsFunc; ///< \warning should not be NULL
};

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
    \param[in] tester that contains tests
    \memberof Tester
*/
void validateAllTests(const Tester* tester);

