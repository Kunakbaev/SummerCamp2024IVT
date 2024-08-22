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
#include <ctype.h>

#include "../include/colourfullPrint.hpp"
#include "../include/quadraticEquation.hpp"

const int MAX_INPUT_LINE_LEN = 25; ///< maximum length of input line

/**
    \warning should be around 1e18, otherwise there might be errors with overflow
    (cause long double MAX is around 1e36 and we use square of inputed values)
*/
const long double MAX_COEF_ABS_VALUE = 1e18; ///< maximum absolute value that coefficient can take


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



// const char* getErrorMessage(QuadEqErrors error) {
//     switch (error) {
//         case QUAD_EQ_NO_ERROR:
//             break;
//         case QUAD_EQ_FILE:
//             break;
//         case QUAD_EQ_ILLEGAL_ARG:
//             break;
//         case QUAD_EQ_VALUE_IS_TOO_BIG:
//             break;
//         case QUAD_EQ_INCORRECT_COEF_FORMAT_ERROR:
//             break;
//         case QUAD_EQ_INCORRECT_COEF_FORMAT_ERROR:
//             break;
//         case QUAD_EQ_LINEAR_EQ_ERROR:
//             break;
//         case QUAD_EQ_INPUT_LINE_TOO_LONG_ERROR:
//             break
//         case QUAD_EQ_INVALID_EQUATION_ERROR:
//             break
//     }
// }



// ------------------------ HELPER FUNCTIONS ---------------------------------------

/// @brief returns square of variable x
static long double square(long double x) {
    return x * x;
}

QuadEqErrors parseLongDoubleAndCheckValid(char* line, long double* coef, bool* result) {
    ///\throw line input line should not be NULL
    ///\throw coef should not be NULL
    ///\throw result should not be NULL
    assert(line != NULL);
    assert(coef != NULL);
    assert(result != NULL);

    if (line == NULL || coef == NULL || result == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

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
    if (*(ptr + 1) == '\n') {
        *(ptr + 1) = '\0';
    } else {
        ++ptr;
    }


    size_t len = strlen(line);
    while (len >= 1 && isblank(*ptr))
        *ptr = '\0', --ptr, --len;

    if (len == 0) {
        *result = false;
        return QUAD_EQ_NO_ERROR;
    }

    errno = 0;
    char* endPtr = NULL; // init NULL (nullptr)
    *coef = strtod(line, &endPtr);
    *result = errno == 0 && *endPtr == '\0';
    return QUAD_EQ_NO_ERROR;
}

/**
    \brief reads coef until it's valid
    \param[in] messageLine Hint for user for what to input
    \result returns long double coefficient

    This function's purpose is too deal with all possible errors that can occur during user's input
*/
static QuadEqErrors getCorrectCoef(const char* messageLine, long double* result) {
    ///\throw line input line should not be NULL
    ///\throw coef should not be NULL
    ///\throw result should not be NULL
    assert(messageLine != NULL);
    assert(result != NULL);

    if (messageLine == NULL || result == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

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
            printError("%s", errorMessages[QUAD_EQ_INPUT_LINE_TOO_LONG_ERROR]);
            continue;
        }

        bool isOk = false;
        QuadEqErrors error = parseLongDoubleAndCheckValid(line, &coef, &isOk);
        if (error) {
            printError("%s", errorMessages[QUAD_EQ_INPUT_LINE_TOO_LONG_ERROR]);
        }

        if (isOk) {
            if (fabsl(coef) > MAX_COEF_ABS_VALUE) {
                printError("%s", errorMessages[QUAD_EQ_VALUE_IS_TOO_BIG]);
            } else {
                isGoodNumber = true;
            }
        } else {
            printError("%s", errorMessages[QUAD_EQ_INCORRECT_COEF_FORMAT_ERROR]);
        }
    } while (!isGoodNumber);

    *result = coef;
    return QUAD_EQ_NO_ERROR;
}




// -------------------- FUNCTIONS REALIZATIONS -------------------------------

/// @brief helper function, gets char sign of coef
static char getSignChar(long double coef) {
    return sign(coef) < 0 ? '-' : '+';
}

QuadEqErrors printEquation(const struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (eq == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    long double a = eq->a, b = eq->b, c = eq->c;
    char bSign = getSignChar(b);
    char cSign = getSignChar(c);
    b = fabsl(b), c = fabsl(c);

    int precision = eq->outputPrecision;
    printf("Your equation is: %.*Lg * x ^ 2 %c %.*Lg * x %c %.*Lg\n",
        precision, a,
        bSign, precision, b,
        cSign, precision, c);
    return QUAD_EQ_NO_ERROR;
}

QuadEqErrors readEquation(struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (eq == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational coefficients\n");
    QuadEqErrors error = getCorrectCoef("Print coefficient A: ", &eq->a);
    if (error) return error;
    error = getCorrectCoef("Print coefficient B: ", &eq->b);
    if (error) return error;
    error = getCorrectCoef("Print coefficient C: ", &eq->c);
    if (error) return error;

    setOutputPrecision(eq, 10);
    return QUAD_EQ_NO_ERROR;
}

/**
    \brief checks if equation is valid
    \param[in] eq function, which value is evaluated
    \result is equation valid
*/
static QuadEqErrors validateEquation(const QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL) return QUAD_EQ_ILLEGAL_ARG;

    long double coefArr[3] = {eq->a, eq->b, eq->c};

    /**
        checks that absolute value of coef is not too big and not too small
        \code
        if (sign(coef - MAX_COEF_ABS_VALUE) > 0 ||
            (sign(coef) == 0 && coef != 0)) {
        \endcode
    */
    for (int i = 0; i < 3; ++i)
        if (sign(fabsl(coefArr[i]) - MAX_COEF_ABS_VALUE) > 0)
            return QUAD_EQ_VALUE_IS_TOO_BIG;

    return QUAD_EQ_NO_ERROR;
}

/**
    \brief gets function value at given point x
    \param[in] eq function, which value is evaluated
    \param[in] x x coordinat, at which value is get
    \result y coordinat, found value at given point
*/
QuadEqErrors getPointValue(const struct QuadraticEquation* eq, long double x, long double* result) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL) return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    ///\warning x should not be too big or too small
    if (fabsl(x) > MAX_COEF_ABS_VALUE)
        return QUAD_EQ_VALUE_IS_TOO_BIG;

    *result = eq->a * square(x) + eq->b * x + eq->c;
    return QUAD_EQ_NO_ERROR;
}

QuadEqErrors setOutputPrecision(struct QuadraticEquation* eq, int outputPrecision) {
    ///\throw eq should not be NULL
    ///\throw outputPrecision should be >= 0
    assert(eq != NULL);
    assert(outputPrecision >= 0);

    if (eq == NULL || outputPrecision < 0)
        return QUAD_EQ_ILLEGAL_ARG;

    eq->outputPrecision = outputPrecision;
    return QUAD_EQ_NO_ERROR;
}

QuadEqErrors getDiscriminant(const struct QuadraticEquation* eq, long double* result) {
    ///\throw eq should not be NULL
    ///\throw result should not be NULL
    assert(eq != NULL);
    assert(result != NULL);

    if (eq == NULL || result == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    *result = square(eq->b) - 4 * eq->a * eq->c;
    return QUAD_EQ_NO_ERROR;
}

QuadEqErrors getVertX(const struct QuadraticEquation* eq, long double* result) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL) return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0) {
        *result = -eq->b / (2 * eq->a);
        return QUAD_EQ_NO_ERROR;
    }

    return QUAD_EQ_LINEAR_EQ_ERROR;
}

QuadEqErrors getVertY(const struct QuadraticEquation* eq, long double* result) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL) return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0) {
        long double disc = 0;
        error = getDiscriminant(eq, &disc);
        if (error) return error;

        *result = -disc / (4 * eq->a);
        return QUAD_EQ_NO_ERROR;
    }
    return QUAD_EQ_LINEAR_EQ_ERROR;
}

static QuadEqErrors solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL);
    assert(answer != NULL);

    if (eq == NULL || answer == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    if (sign(eq->b) == 0) {
        answer->numOfSols = sign(eq->c) ? NO_ROOTS : INFINITE_ROOTS;
        return QUAD_EQ_NO_ERROR;
    }

    answer->root_1 = -eq->c / eq->b;
    answer->numOfSols = ONE_ROOT;
    return QUAD_EQ_NO_ERROR;
}

/**
    \brief solves not linear case of equation (a != 0)
    \param[in] eq given equation
    \param[out] answer found roots and info about their cnt
*/
static QuadEqErrors solveQuadraticEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL);
    assert(answer != NULL);

    if (eq == NULL || answer == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    long double disc = 0;
    error = getDiscriminant(eq, &disc);
    if (error) return error;

    /// negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return QUAD_EQ_NO_ERROR;
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

    return QUAD_EQ_NO_ERROR;
}

QuadEqErrors getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL);
    assert(answer != NULL);

    if (eq == NULL || answer == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    if (sign(eq->a) == 0)
        return solveLinearEquation(eq, answer);
    return solveQuadraticEquation(eq, answer);
}

QuadEqErrors printSolutions(const struct QuadraticEquationAnswer* answer, int outputPrecision, const char* outputFile) {
    ///\throw answer should not be NULL
    assert(answer != NULL);
    if (answer == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    FILE* stream = stdout;
    if (outputFile != NULL) {
        FILE* outFile = fopen(outputFile, "w");
        assert(outFile != NULL);
        if (outFile == NULL)
            return QUAD_EQ_INVALID_FILE;

        changeTextColor(YELLOW_COLOR);
        colourfullPrint("Output of solutions goes to file: %s\n", outputFile);
        stream = outFile;
    }

    if (answer->numOfSols == INFINITE_ROOTS) {
        fprintf(stream, "Infinetly many solutions\n");
        return QUAD_EQ_NO_ERROR;
    }

    fprintf(stream, "Number of solutions: %d, solutions of equation : { ", answer->numOfSols);
    if (answer->numOfSols != NO_ROOTS)
        fprintf(stream, "%.*Lg", outputPrecision, answer->root_1);
    if (answer->numOfSols == TWO_ROOTS)
        fprintf(stream, ", %.*Lg", outputPrecision, answer->root_2);
    fprintf(stream, " }\n");

    if (outputFile != NULL)
        fclose(stream);
    return QUAD_EQ_NO_ERROR;
}

QuadEqErrors solveAndPrintEquation(const struct QuadraticEquation* eq, const char* outputFile) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (eq == NULL)
        return QUAD_EQ_ILLEGAL_ARG;

    QuadEqErrors error = validateEquation(eq);
    if (error) return error;

    struct QuadraticEquationAnswer answer;
    error = getSolutions(eq, &answer);
    if (error) return error;
    return printSolutions(&answer, eq->outputPrecision, outputFile);
}

