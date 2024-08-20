/**
    \file
    \brief class containing all methods from lib
    In this file prototypes of all methods of class QuadraticEquation are declared
*/

// -------------------------- QUADRATIC EQUATION "CLASS" ---------------------------------------


/**
    \brief main struct

    Quadratic equation looks like this: a * x ^ 2 + b * x + c.

    Class can find solutions of quadratic equation in rationals,
    also outputPrecisoin can be state (10 by default), helps outputing floating number.
*/
struct QuadraticEquation {
    int outputPrecision = 10; ///< states maximum number of digits of float numbers that will be outputed
    long double a = 0, b = 0, c = 0; ///< a, b, c are coefficients, some rational numbers
};

/**
    \brief This enum is used to understand what type of equation we have
*/
enum QuadEqRootState {
    NO_ROOTS, ///< equation has no solutions at all
    ONE_ROOT, ///< equation has only one root
    TWO_ROOTS, ///< equation has two roots
    INFINITE_ROOTS, ///< equation has infinetly many solutions
};

/**
    \brief This struct is used as return type for getSolutions() method
    root_1, root_2 -> roots of equation
    numOfSols -> shows how many solutions we have
*/

struct QuadraticEquationAnswer {
    long double root_1 = 0; ///< first root of equation
    long double root_2 = 0; ///< second root of equation
    enum QuadEqRootState numOfSols = NO_ROOTS; ///< number of solutions equation has
};

// structure "methods"
void readEquation(struct QuadraticEquation* eq); ///< \memberof QuadraticEquation
void printEquation(const struct QuadraticEquation* eq); ///< \memberof QuadraticEquation
long double getPointValue(const struct QuadraticEquation* eq, long double x); ///< \memberof QuadraticEquation
long double getDiscriminant(const struct QuadraticEquation* eq); ///< \memberof QuadraticEquation
long double getVertX(const struct QuadraticEquation* eq); ///< \memberof QuadraticEquation
long double getVertY(const struct QuadraticEquation* eq); ///< \memberof QuadraticEquation
void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer); ///< \memberof QuadraticEquation
void printSolutions(const struct QuadraticEquationAnswer answer, int outputPrecision); ///< \memberof QuadraticEquation
void solveAndPrintEquation(const struct QuadraticEquation* eq); ///< \memberof QuadraticEquation

