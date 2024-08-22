/**

    \file
    \brief class containing realization of all methods from QuadraticEquation lib

    This is main file, with all functionality of class. File contains realization of all methods of class QuadraticEquation

*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "../colourfullPrintLib/colourfullPrint.hpp"
#include "quadraticEquation.hpp" // FIXME: pochitat' ob posledovatelnosti include

const int MAX_INPUT_LINE_LEN = 25; ///< maximum length of input line

/**
    \warning should be around 1e18, otherwise there might be errors with overflow
    (cause long double MAX is around 1e36 and we use square of inputed values)
*/
const long double MAX_COEF_ABS_VALUE = 1e18; ///< maximum absolute value that coefficient can take

/// @brief error occures if absolute value of number is too big
const char* VALUE_IS_TOO_BIG_ERROR =      "Error: absolute value of inputed number is too big\n";

/// @brief error occures if inputed long double number is given in incorrect format
const char* INCORRECT_COEF_FORMAT_ERROR = "Error: that's not a correct number\n";

/// @brief error occures if user tries to use functions that are only can be used if equation is square equation (a != 0)
const char* LINEAR_EQ_ERROR =             "Error: this function can not be used with a linear equation\n";

/// @brief error occures if input line length exceeds MAX_COEF_ABS_VALUE
const char* INPUT_LINE_TOO_LONG_ERROR =   "Error: input line is too long\n";

/// @brief error occures if user changed coefficients to be invalid
const char* INVALID_EQUATION_ERROR = "Error: coefficients of equation are invalid\n";


/**
    epsilon, regulates with what precision we work
    \warning Should not be too small, otherwise there might be some errors with precision
*/
const long double EPSILON = 1e-9; ///< regulates with what precision we work

/// @brief returns sign of variable x, we use it to avoid some precision problems
int sign(long double x) {
    if (x < -EPSILON) return -1;
    return x > EPSILON;
}

// ------------------------ HELPER FUNCTIONS ---------------------------------------

/// @brief returns square of variable x
static long double square(long double x) {
    return x * x;
}

bool parseLongDoubleAndCheckValid(char* line, long double* coef) {
    ///\throw line input line should not be NULL
    ///\throw coef should not be NULL
    assert(line != NULL && coef != NULL);

    /**
        trims string while last char == space or tab
        \code
        char* ptr = line + strlen(line) - 2;
        *(ptr + 1) = '\0';
        while (strlen(line) >= 2 &&
            (*ptr == ' ' || *ptr == '\t'))
                *ptr = '\0', --ptr;
        \endcode
    */
    char* ptr = line + strlen(line) - 2; // lineEnd
    //  *ptr == \n: assert
    // assert(*ptr == '\n');

    if (*ptr == '\n') {
        *(ptr + 1) = '\0';
    } else {
        ++ptr;
    }

    size_t len = strlen(line);
    while (len >= 2 &&
        (*ptr == ' ' || *ptr == '\t')) // FIXME: change to func
            *ptr = '\0', --ptr, --len;

    errno = 0;
    char* endPtr; // init NULL (nullptr)
    *coef = strtod(line, &endPtr);
    return errno == 0 && *endPtr == '\0';
}

/**
    \brief reads coef until it's valid
    \param[in] messageLine Hint for user for what to input
    \result returns long double coefficient

    This function's purpose is too deal with all possible errors that can occur during user's input
*/
static long double getCorrectCoef(const char* messageLine) {
    ///\throw line input line should not be NULL
    ///\throw coef should not be NULL
    assert(messageLine != NULL);

    long double coef = 0;
    bool isGoodNumber = false;

    /**
        \brief do while loop, until good coefficient will be given
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
        if (parseLongDoubleAndCheckValid(line, &coef)) {...
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
            printError("%s", INPUT_LINE_TOO_LONG_ERROR);
            continue;
        }

        if (parseLongDoubleAndCheckValid(line, &coef)) {
            if (fabsl(coef) > MAX_COEF_ABS_VALUE) {
                printError("%s", VALUE_IS_TOO_BIG_ERROR);
            } else {
                isGoodNumber = true;
            }
        } else {
            printError("%s", INCORRECT_COEF_FORMAT_ERROR);
        }
    } while (!isGoodNumber);

    return coef;
}




// -------------------- FUNCTIONS REALIZATIONS -------------------------------

/// @brief helper function, gets char sign of coef
static char getSignChar(long double coef) {
    return sign(coef) < 0 ? '-' : '+';
}

void printEquation(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    long double a = eq->a, b = eq->b, c = eq->c; // FIXME:
    char bSign = getSignChar(b);
    char cSign = getSignChar(c);
    b = fabsl(b), c = fabsl(c);

    int precision = eq->outputPrecision;
    printf("Your equation is: %.*Lg * x ^ 2 %c %.*Lg * x %c %.*Lg\n",
        precision, a,
        bSign, precision, b,
        cSign, precision, c);
}

void readEquation(struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational coefficients\n");
    eq->a = getCorrectCoef("Print coefficient A: ");
    eq->b = getCorrectCoef("Print coefficient B: ");
    eq->c = getCorrectCoef("Print coefficient C: ");

    setOutputPrecision(eq, 10);
}

/**
    \brief checks if equation is valid
    \param[in] eq function, which value is evaluated
    \result is equation valid
*/
static bool validateEquation(const QuadraticEquation* eq) {
    long double coefArr[3] = {eq->a, eq->b, eq->c};

    /**
        checks that absolute value of coef is not too big and not too small
        \code
        if (sign(coef - MAX_COEF_ABS_VALUE) > 0 ||
            (sign(coef) == 0 && coef != 0)) {
        \endcode
    */
    for (int i = 0; i < 3; ++i) {
        long double coef = fabsl(coefArr[i]);
        if (sign(coef - MAX_COEF_ABS_VALUE) > 0) {
            printError("%s", INVALID_EQUATION_ERROR);
            return false;
        }
    }

    return true;
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

    if (!validateEquation(eq)) return 0;

    ///\warning x should not be too big or too small
    if (fabsl(x) > MAX_COEF_ABS_VALUE) {
        printError("%s", VALUE_IS_TOO_BIG_ERROR);
        return 0;
    }

    return eq->a * square(x) + eq->b * x + eq->c;
}

void setOutputPrecision(struct QuadraticEquation* eq, int outputPrecision) {
    ///\throw eq should not be NULL
    ///\throw outputPrecision should be >= 0
    assert(eq != NULL);
    assert(outputPrecision >= 0);

    eq->outputPrecision = outputPrecision;
}

long double getDiscriminant(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (!validateEquation(eq)) return 0;
    return square(eq->b) - 4 * eq->a * eq->c;
}

long double getVertX(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (!validateEquation(eq)) return 0;

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0)
        return -eq->b / (2 * eq->a);
    printError("%s", LINEAR_EQ_ERROR);
    return 0;
}

long double getVertY(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (!validateEquation(eq)) return 0;

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0)
        return -getDiscriminant(eq) / (4 * eq->a);
    printError("%s", LINEAR_EQ_ERROR);
    return 0;
}

static void solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL && answer != NULL);

    if (!validateEquation(eq)) return;

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
*/
static void solveQuadraticEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL && answer != NULL);

    if (!validateEquation(eq)) return;

    long double disc = getDiscriminant(eq);
    /// negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return;
    }

    /**
        we store this values, because they take a lot of time to compute (espessialy operation of square root).
        So in case if we have a big number of calls to this function, it will not work too slow.
        \code
        long double discRoot = sqrtl(disc);
        long double denom = 1 / (2 * eq->a);
        \endcode
    */

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

void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL && answer != NULL);

    if (!validateEquation(eq)) return;

    if (sign(eq->a) == 0) {
        solveLinearEquation(eq, answer);
        return;
    }

    solveQuadraticEquation(eq, answer);
}

void printSolutions(const struct QuadraticEquationAnswer* answer, int outputPrecision, const char* outputFile) {
    ///\throw answer should not be NULL
    assert(answer != NULL);

    FILE* stream = stdout;
    if (strlen(outputFile) != 0) {
        FILE* outFile;
        outFile = fopen(outputFile, "w");
        assert(outFile != NULL);
        printf("Output of solutions goes to file: %s\n", outputFile);
        stream = outFile;
    }

    if (answer->numOfSols == INFINITE_ROOTS) { // FIXME: rewrite. Пусть он пишет сколько решений
        fprintf(stream, "Infinetly many solutions\n");
        return;
    }

    fprintf(stream, "Number of solutions: %d, solutions of equation : { ", answer->numOfSols);
    if (answer->numOfSols != NO_ROOTS)
        fprintf(stream, "%.*Lg", outputPrecision, answer->root_1);
    if (answer->numOfSols == TWO_ROOTS)
        fprintf(stream, ", %.*Lg", outputPrecision, answer->root_2);
    fprintf(stream, " }\n");

    if (strlen(outputFile) != 0)
        fclose(stream);
}

void solveAndPrintEquation(const struct QuadraticEquation* eq, const char* outputFile) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (!validateEquation(eq)) return;

    struct QuadraticEquationAnswer answer;
    getSolutions(eq, &answer);
    printSolutions(&answer, eq->outputPrecision, outputFile);
}

