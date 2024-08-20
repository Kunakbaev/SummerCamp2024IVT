/**

    \file
    \brief class containing realization of all methods from lib

    This is main file, with all functionality of class. File contains realization of all methods of class QuadraticEquation

*/


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "quadraticEquationLib.hpp"



/**

    epsilon, regulates with what precision we work

    Should not be too small, otherwise there might be some errors with precision

*/
const long double EPSILON = 1e-9;
const int MAX_INPUT_LINE_LEN = 25; ///< maximum length of input line

/**

    should be around 1e18, otherwise there might be errors with overflow
    (cause long double MAX is around 1e36 and we use square of inputed values)

*/
const long double MAX_KOEF_ABS_VALUE = 1e18;

/// @brief error occures if absolute value of number is too big
const char* VALUE_IS_TOO_BIG_ERROR =      "Error: absolute value of inputed number is too big\n";

/// @brief error occures if inputed long double number is given in incorrect format
const char* INCORRECT_KOEF_FORMAT_ERROR = "Error: that's not a correct number\n";

/// @brief error occures if user tries to use functions that are only can be used if equation is square equation (a != 0)
const char* LINEAR_EQ_ERROR =             "Error: this function can not be used with a linear equation\n";

/// @brief error occures if input line length exceeds MAX_KOEF_ABS_VALUE
const char* INPUT_LINE_TOO_LONG_ERROR =   "Error: input line is too long\n";


// ------------------------ HELPER FUNCTIONS ---------------------------------------

/// @brief returns sign of variable x, we use it to avoid some precision problems
static int sign(long double x) {
    if (x < -EPSILON) return -1;
    return x > EPSILON;
}

/// @brief returns square of variable x
static long double square(long double x) {
    return x * x;
}

/**
    \brief parses long double from string and checks if it's valid

    \param[in] line Input line that user gave in console
    \param[out] koef Stores parsed long double koefficient
    \result boolean true, if given input line is valid
*/
static bool parseLongDoubleAndCheckValid(const char line[], long double* koef) {
    ///\throw line input line should not be NULL
    ///\throw koef should not be NULL
    assert(line != NULL && koef != NULL);

    errno = 0;
    char* endPtr;
    *koef = strtod(line, &endPtr);
    return errno == 0 && *endPtr == '\n';
}

/**
    \brief reads koef until it's valid
    \param[in] messageLine Hint for user for what to input
    \result returns long double koefficient

    This function's purpose is too deal with all possible errors that can occur during user's input
*/
static long double getCorrectKoef(const char messageLine[]) {
    ///\throw line input line should not be NULL
    ///\throw koef should not be NULL
    assert(messageLine != NULL);

    long double koef = 0;
    bool isGoodNumber = false;

    /**
        \brief do while loop, until good koefficient will be given
        \code
        ...} while (!isGoodNumber);
        \endcode

        \brief line is readed until its length is small enough
        \code
        do {
            fgets(line, MAX_INPUT_LINE_LEN, stdin);
            inputLineLen += strlen(line);
        } while (line[strlen(line) - 1] != '\n');
        \endcode

        \brief check for valid input format
        \code
        if (parseLongDoubleAndCheckValid(line, &koef)) {...
        \endcode
    */

    do {
        printf("%s", messageLine);

        size_t inputLineLen = 0;
        char line[MAX_INPUT_LINE_LEN];
        do { // line is readed until its length is small enough
            fgets(line, MAX_INPUT_LINE_LEN, stdin);
            inputLineLen += strlen(line);
        } while (line[strlen(line) - 1] != '\n');

        if (inputLineLen - 1 > MAX_INPUT_LINE_LEN) {
            fprintf(stderr, "%s", INPUT_LINE_TOO_LONG_ERROR);
            continue;
        }

        if (parseLongDoubleAndCheckValid(line, &koef)) {
            if (fabsl(koef) > MAX_KOEF_ABS_VALUE)
                fprintf(stderr, "%s", VALUE_IS_TOO_BIG_ERROR);
            else
                isGoodNumber = true;
        } else {
            fprintf(stderr, "%s", INCORRECT_KOEF_FORMAT_ERROR);
        }
    } while (!isGoodNumber);

    return koef;
}




// -------------------- FUNCTIONS REALIZATIONS -------------------------------

/// @brief helper function, gets char sign of koef
static char getSignChar(long double koef) {
    return sign(koef) < 0 ? '-' : '+';
}

/**
    \brief prints given equation
    \param[in] eq Quadratic equation that will be printed
    \result void
*/
void printEquation(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    long double a = eq->a, b = eq->b, c = eq->c;
    char bSign = getSignChar(b);
    char cSign = getSignChar(c);
    b = fabsl(b), c = fabsl(c);

    int precision = eq->outputPrecision;
    printf("Your equation is: %.*Lg * x ^ 2 %c %.*Lg * x %c %.*Lg\n",
        precision, a,
        bSign, precision, b,
        cSign, precision, c);
}


/**
    \brief reads given equation
    \param[out] eq equation that will be read
    \result void
*/
void readEquation(struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational koefficients\n");
    eq->a = getCorrectKoef("Print koefficient A: ");
    eq->b = getCorrectKoef("Print koefficient B: ");
    eq->c = getCorrectKoef("Print koefficient C: ");
}

/**
    \brief gets function value at given point x
    \param[in] eq function, which value is evaluated
    \param[in] x x coordinat, at which value is get
    \result y coordinat, found value at given point
*/
long double getPointValue(const struct QuadraticEquation* eq, long double x) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (fabsl(x) > MAX_KOEF_ABS_VALUE) {
        fprintf(stderr, "%s", VALUE_IS_TOO_BIG_ERROR);
        return 0;
    }
    return eq->a * square(x) + eq->b * x + eq->c;
}

/**
    \brief returns discriminant of given equation
    \param[in] eq given equation
    \result long double, found discriminant
*/
long double getDiscriminant(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    return square(eq->b) - 4 * eq->a * eq->c;
}

/**
    \brief returns x coordinat of top of the parabola
    \param[in] eq given equation
    \result long double, x coordinat of top of the parabola
*/
long double getVertX(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0)
        return -eq->b / (2 * eq->a);
    fprintf(stderr, "%s", LINEAR_EQ_ERROR);
    return 0;
}

/**
    \brief returns y coordinat of top of the parabola
    \param[in] eq given equation
    \result long double, y coordinat of top of the parabola
*/
long double getVertY(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0)
        return -getDiscriminant(eq) / (4 * eq->a);
    fprintf(stderr, "%s", LINEAR_EQ_ERROR);
    return 0;
}


/**
    \brief solves linear case of equation (a == 0)
    \param[in] eq given equation
    \param[out] answer found roots and info about their cnt
    \result void
*/
static void solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL && answer != NULL);

    if (sign(eq->b) == 0) {
        answer->numOfSols = sign(eq->c) ? NO_ROOTS : INFINITE_ROOTS;
        return;
    }

    answer->root_1 = -eq->c / eq->b;
    answer->numOfSols = ONE_ROOT;
}

/**
    \brief solves not linear case of equation (a != 0)
    \param[in] eq given equation
    \param[out] answer found roots and info about their cnt
    \result void
*/
static void solveQuadraticEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL && answer != NULL);

    long double disc = getDiscriminant(eq);
    /// negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return;
    }

    long double discRoot = sqrtl(disc);
    long double denom = 1 / (2 * eq->a);
    answer->root_1 = (-eq->b - discRoot) * denom;
    /// we have 2 distinct solutions in case if disc != 0
    if (sign(disc) != 0) {
        answer->root_2 = (-eq->b + discRoot) * denom;
        answer->numOfSols = TWO_ROOTS;
    } else {
        answer->numOfSols = ONE_ROOT;
    }
}

/**
    \brief core function, gets solutions of quadratic equation
    \param[in] eq given equation
    \param[out] answer found roots and info about their cnt
    \result void
*/
void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL && answer != NULL);

    if (sign(eq->a) == 0) {
        solveLinearEquation(eq, answer);
        return;
    }

    solveQuadraticEquation(eq, answer);
}

/**
    \brief prints found solutions
    \param[in] answer found roots and info about their cnt
    \param[in] outputPrecision maximum number of digits after comma
    \result void
*/
void printSolutions(const struct QuadraticEquationAnswer answer, int outputPrecision = 10) {
    if (answer.numOfSols == INFINITE_ROOTS) {
        printf("Infinetly many solutions\n");
        return;
    }

    printf("Solutions of equation : { ");
    if (answer.numOfSols != NO_ROOTS)
        printf("%.*Lg", outputPrecision, answer.root_1);
    if (answer.numOfSols == TWO_ROOTS)
        printf(", %.*Lg", outputPrecision, answer.root_2);
    printf(" }\n");
}

/**
    \brief solves equation and prints found solutions
    \param[in] eq given equation
    \result void
*/
void solveAndPrintEquation(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    struct QuadraticEquationAnswer answer;
    getSolutions(eq, &answer);
    printSolutions(answer, eq->outputPrecision);
}

