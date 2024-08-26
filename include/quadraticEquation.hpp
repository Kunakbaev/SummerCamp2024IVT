#ifndef QUADRATIC_EQUATION_HEADER
#define QUADRATIC_EQUATION_HEADER


/**
    \file
    \brief class containing all methods from quadratic equation lib
    In this file prototypes of all methods of class QuadraticEquation are declared
*/

const int DEFAULT_PRECISION = 10;

/// @brief enum that contains errors
enum QuadEqErrors { // FIXME: add _ERROR
    QUAD_EQ_ERRORS_OK                    = 0,
    QUAD_EQ_ERRORS_INVALID_FILE          = 1,
    QUAD_EQ_ERRORS_ILLEGAL_ARG           = 2,
    QUAD_EQ_ERRORS_VALUE_IS_TOO_BIG      = 3,
    QUAD_EQ_ERRORS_INCORRECT_COEF_FORMAT = 4,
    QUAD_EQ_ERRORS_LINEAR_EQ             = 5,
    QUAD_EQ_ERRORS_INPUT_LINE_TOO_LONG   = 6,
    QUAD_EQ_ERRORS_INVALID_EQUATION      = 7
};

/**
    \brief returns string represetatin of error
    \param[in] error Enum, type of error
    \result returns string representatin of error
*/
const char* getErrorMessage(QuadEqErrors error);

// sign function is need in multiple files so it needs to be extern
int sign(long double x);

/**
    \brief parses long double from string and checks if it's valid

    \param[in] line Input line that user gave in console
    \param[out] coef Stores parsed long double coefficient
    \result true, if given input line is valid
*/
QuadEqErrors parseLongDoubleAndCheckValid(char* line, long double* coef, bool* result);



// -------------------------- QUADRATIC EQUATION "CLASS" ---------------------------------------


/**
    \brief main struct

    Quadratic equation looks like this: a * x ^ 2 + b * x + c.

    Class can find solutions of quadratic equation in rationals,
    also outputPrecisoin can be state (10 by default), helps outputing floating number.
*/
struct QuadraticEquation {
    long double a; ///< coefficient at x^2
    long double b; ///< coefficient at x
    long double c; ///< "free" coefficient
    int outputPrecision; ///< states maximum number of digits of float numbers that will be outputed
};

/**
    \brief This enum is used to understand what type of equation we have
*/
enum QuadEqRootState {
    NO_ROOTS = 0, ///< equation has no solutions at all
    ONE_ROOT = 1, ///< equation has only one root
    TWO_ROOTS = 2, ///< equation has two roots
    INFINITE_ROOTS, ///< equation has infinetly many solutions
};

/**
    \brief This struct is used as return type for getSolutions() method
    root_1, root_2 -> roots of equation
    numOfSols -> shows how many solutions we have
*/

struct QuadraticEquationAnswer {
    long double root_1; ///< first root of equation
    long double root_2; ///< second root of equation
    QuadEqRootState numOfSols; ///< number of solutions equation has
};





// structure "methods"

/**
    \brief reads given equation
    \param[out] eq equation that will be read
    \result void
*/
QuadEqErrors readEquation(struct QuadraticEquation* eq); ///< \memberof QuadraticEquation

/**
    \brief prints given equation
    \param[in] eq Quadratic equation that will be printed
    \result void
*/
QuadEqErrors printEquation(const struct QuadraticEquation* eq); ///< \memberof QuadraticEquation

/**
    \brief gets function value at given point x
    \param[in] eq function, which value is evaluated
    \param[in] x x coordinat, at which value is get
    \result y coordinat, found value at given point
*/
QuadEqErrors getPointValue(const struct QuadraticEquation* eq, long double x, long double* result); ///< \memberof QuadraticEquation

/**
    \brief sets output precision of give equation
    \param[in] eq give equation
    \param[in] outputPrecision precision when outputing numbers
*/
QuadEqErrors setOutputPrecision(struct QuadraticEquation* eq, int outputPrecision); ///< \memberof QuadraticEquation

/**
    \brief returns discriminant of given equation
    \param[in] eq given equation
    \result found discriminant
*/
QuadEqErrors getDiscriminant(const struct QuadraticEquation* eq, long double* result); ///< \memberof QuadraticEquation

/**
    \brief returns x coordinat of top of the parabola
    \param[in] eq given equation
    \result x coordinat of top of the parabola
*/
QuadEqErrors getVertX(const struct QuadraticEquation* eq, long double* result); ///< \memberof QuadraticEquation

/**
    \brief returns y coordinat of top of the parabola
    \param[in] eq given equation
    \result y coordinat of top of the parabola
*/
QuadEqErrors getVertY(const struct QuadraticEquation* eq, long double* result); ///< \memberof QuadraticEquation

/**
    \brief core function, gets solutions of quadratic equation
    \param[in] eq given equation
    \param[out] answer found roots and info about their cnt
*/
QuadEqErrors getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer); ///< \memberof QuadraticEquation

/**
    \brief prints found solutions
    \param[in] answer found roots and info about their cnt
    \param[in] outputPrecision maximum number of digits after comma
    \param[in] outputFile can be equal to "", if len > 0 then output goes to file
    \warning outputFile should be a correct file name
*/
QuadEqErrors printSolutions(const struct QuadraticEquationAnswer* answer, int outputPrecision, const char* outputFile); ///< \memberof QuadraticEquation

/**
    \brief solves equation and prints found solutions
    \param[in] eq given equation
    \param[in] outputFile can be equal to NULL (means that output goes to stdout), if len > 0 then output goes to file
    \warning outputFile should be a correct file name
*/
QuadEqErrors solveAndPrintEquation(const struct QuadraticEquation* eq, const char* outputFile); ///< \memberof QuadraticEquation

#endif
