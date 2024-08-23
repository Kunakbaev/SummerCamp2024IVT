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

/*
#define LOG_AND_RETURN(...) printError("Error occured, in FILE: %s, FUNCTION: %s, at LINE: %d\n", __FILE__, __FUNCTION__, __LINE__); \
                            printError(__VA_ARGS__); \
*/

#define LOG_AND_RETURN(ERROR, ...)                                                                               \
    do {                                                                                                         \
        printError("Error occured, in FILE: %s, FUNCTION: %s, at LINE: %d\n", __FILE__, __FUNCTION__, __LINE__); \
        printError("%s", getErrorMessage(ERROR));                                                                \
        return ERROR;                                                                                            \
    } while(0)


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



const char* getErrorMessage(QuadEqErrors error) {
    switch (error) {
        case QUAD_EQ_ERRORS_OK:
            return "No errors occured\n";
        case QUAD_EQ_ERRORS_INVALID_FILE:
            return "Error: couldn't open file\n";
        case QUAD_EQ_ERRORS_ILLEGAL_ARG:
            return "Error: illegal argument (possibly set to NULL)\n";
        case QUAD_EQ_ERRORS_VALUE_IS_TOO_BIG:
            return "Error: absolute value of inputed number is too big\n";
        case QUAD_EQ_ERRORS_INCORRECT_COEF_FORMAT:
            return "Error: that's not a correct number\n";
        case QUAD_EQ_ERRORS_LINEAR_EQ:
            return "Error: this function can not be used with a linear equation\n";
        case QUAD_EQ_ERRORS_INPUT_LINE_TOO_LONG:
            return "Error: input line is too long\n";
        case QUAD_EQ_ERRORS_INVALID_EQUATION:
            return "Error: coefficients of equation are invalid\n";
        default:
            return "No errors occured\n";
    }
}



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
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    char* ptr = line + strlen(line) - 2; // lineEnd
    //  *ptr == \n: assert
    // assert(*ptr == '\n');
    if (*(ptr + 1) == '\n') {
        *(ptr + 1) = '\0';
    } else {
        ++ptr;
    }

    size_t len = strlen(line);
    while (len >= 1 && isblank(*ptr)) {
        *ptr = '\0';
        --ptr;
        --len;
    }

    if (len == 0) {
        *result = false;
        return QUAD_EQ_ERRORS_OK;
    }

    errno = 0;
    char* endPtr = NULL; // init NULL (nullptr)
    *coef = strtod(line, &endPtr);
    *result = ((errno == 0) && (*endPtr == '\0'));
    return QUAD_EQ_ERRORS_OK;
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
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    long double coef = 0;
    bool isGoodNumber = false;

    do {
        printf("%s", messageLine);

        size_t inputLineLen = 0;
        char line[MAX_INPUT_LINE_LEN] = "";
        do { // line is readed until its length is small enough
            fgets(line, MAX_INPUT_LINE_LEN, stdin);
            inputLineLen += strlen(line);
        } while (line[strlen(line) - 1] != '\n');

        if (inputLineLen - 1 > MAX_INPUT_LINE_LEN) {
            printError("%s", getErrorMessage(QUAD_EQ_ERRORS_INPUT_LINE_TOO_LONG));
            continue;
        }

        bool isOk = false;
        QuadEqErrors error = parseLongDoubleAndCheckValid(line, &coef, &isOk);
        if (error != QUAD_EQ_ERRORS_OK) //FIXME :
            printError("%s", getErrorMessage(QUAD_EQ_ERRORS_INPUT_LINE_TOO_LONG));

        if (isOk) {
            if (fabsl(coef) > MAX_COEF_ABS_VALUE)
                printError("%s", getErrorMessage(QUAD_EQ_ERRORS_VALUE_IS_TOO_BIG));
            else
                isGoodNumber = true;
        } else {
            printError("%s", getErrorMessage(QUAD_EQ_ERRORS_INCORRECT_COEF_FORMAT));
        }
    } while (!isGoodNumber);

    *result = coef;
    return QUAD_EQ_ERRORS_OK;
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
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    long double a = eq->a;
    long double b = fabsl(eq->b);
    long double c = fabsl(eq->c);
    char bSign = getSignChar(eq->b);
    char cSign = getSignChar(eq->c);
    // printf("Bruh : %c %c\n", bSign, cSign);

    //LOG_AND_RETURN("aaaaaahhhhhhhhhh bruuuuuuuh\n");

    int precision = eq->outputPrecision;
    printf("Your equation is: %.*Lg * x ^ 2 %c %.*Lg * x %c %.*Lg\n",
            precision, a,
            bSign, precision, b,
            cSign, precision, c);
    return QUAD_EQ_ERRORS_OK;
}

QuadEqErrors readEquation(struct QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (eq == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational coefficients\n");
    QuadEqErrors error = getCorrectCoef("Print coefficient A: ", &eq->a);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);
    error = getCorrectCoef("Print coefficient B: ", &eq->b);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);
    error = getCorrectCoef("Print coefficient C: ", &eq->c);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    setOutputPrecision(eq, DEFAULT_PRECISION);
    return QUAD_EQ_ERRORS_OK;
}

/**
    \brief checks if equation is valid
    \param[in] eq function, which value is evaluated
    \result is equation valid
*/
static QuadEqErrors validateEquation(const QuadraticEquation* eq) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    long double coefArr[3] = {eq->a, eq->b, eq->c};
    for (int i = 0; i < 3; ++i)
        if (sign(fabsl(coefArr[i]) - MAX_COEF_ABS_VALUE) > 0)
            return QUAD_EQ_ERRORS_VALUE_IS_TOO_BIG;
    return QUAD_EQ_ERRORS_OK;
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
    if (eq == NULL) // FIXME: delete curly braces
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    ///\warning x should not be too big or too small
    if (fabsl(x) > MAX_COEF_ABS_VALUE)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_VALUE_IS_TOO_BIG);

    *result = eq->a * square(x) + eq->b * x + eq->c;
    return QUAD_EQ_ERRORS_OK;
}

QuadEqErrors setOutputPrecision(struct QuadraticEquation* eq, int outputPrecision) {
    ///\throw eq should not be NULL
    ///\throw outputPrecision should be >= 0
    assert(eq != NULL);
    assert(outputPrecision >= 0);

    if (eq == NULL || outputPrecision < 0)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    eq->outputPrecision = outputPrecision;
    return QUAD_EQ_ERRORS_OK;
}

QuadEqErrors getDiscriminant(const struct QuadraticEquation* eq, long double* result) {
    ///\throw eq should not be NULL
    ///\throw result should not be NULL
    assert(eq != NULL);
    assert(result != NULL);

    if (eq == NULL || result == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    *result = square(eq->b) - 4 * eq->a * eq->c;
    return QUAD_EQ_ERRORS_OK;
}

QuadEqErrors getVertX(const struct QuadraticEquation* eq, long double* result) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0) {
        *result = -eq->b / (2 * eq->a);
        return QUAD_EQ_ERRORS_OK;
    }


    LOG_AND_RETURN(QUAD_EQ_ERRORS_LINEAR_EQ);
}

QuadEqErrors getVertY(const struct QuadraticEquation* eq, long double* result) {
    ///\throw eq should not be NULL
    assert(eq != NULL);
    if (eq == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_OK);

    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    ///\warning eq->a should not be 0
    if (sign(eq->a) != 0) {
        long double disc = 0;
        error = getDiscriminant(eq, &disc);
        if (error != QUAD_EQ_ERRORS_OK)
            LOG_AND_RETURN(error);

        *result = -disc / (4 * eq->a);
        return QUAD_EQ_ERRORS_OK;
    }

    LOG_AND_RETURN(QUAD_EQ_ERRORS_LINEAR_EQ);
}

static QuadEqErrors solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL);
    assert(answer != NULL);

    if (eq == NULL || answer == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    if (sign(eq->b) == 0) {
        answer->numOfSols = sign(eq->c) ? NO_ROOTS : INFINITE_ROOTS;
        return QUAD_EQ_ERRORS_OK;
    }

    answer->root_1 = answer->root_2 = -eq->c / eq->b;
    answer->numOfSols = ONE_ROOT;
    return QUAD_EQ_ERRORS_OK;
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
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    long double disc = 0;
    error = getDiscriminant(eq, &disc);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    /// negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return QUAD_EQ_ERRORS_OK;
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

        // WARNING !!!
        answer->root_2 = answer->root_1;
    }

    return QUAD_EQ_ERRORS_OK;
}

QuadEqErrors getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    ///\throw eq should not be NULL
    ///\throw answer should not be NULL
    assert(eq != NULL);
    assert(answer != NULL);

    if (eq == NULL || answer == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    // FIXME: wrap around validate
    // FIXME: midway logging
    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    if (sign(eq->a) == 0)
        return solveLinearEquation(eq, answer);
    return solveQuadraticEquation(eq, answer);
}

QuadEqErrors printSolutions(const struct QuadraticEquationAnswer* answer, int outputPrecision, const char* outputFile) {
    ///\throw answer should not be NULL
    assert(answer != NULL);
    if (answer == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    FILE* stream = stdout;
    if (outputFile != NULL) {
        FILE* outFile = fopen(outputFile, "w");
        assert(outFile != NULL);
        if (outFile == NULL)
            LOG_AND_RETURN(QUAD_EQ_ERRORS_INVALID_FILE);

        changeTextColor(YELLOW_COLOR);
        colourfullPrint("Output of solutions goes to file: %s\n", outputFile);
        stream = outFile;
    }

    if (answer->numOfSols != INFINITE_ROOTS)
        fprintf(stream, "Number of solutions: %d, solutions of equation : { ", answer->numOfSols);

    //WARNING: root_1 can be equal to root_2
    switch (answer->numOfSols) {
        case INFINITE_ROOTS:
            fprintf(stream, "Infinitely many solutions\n");
            break;
        case NO_ROOTS:
            break;
        case TWO_ROOTS:
            fprintf(stream, "%.*Lg, ", outputPrecision, answer->root_1);
        case ONE_ROOT:
            fprintf(stream, "%.*Lg", outputPrecision, answer->root_2);
            break;
        default:
            assert(false);
            LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);
    }
    if (answer->numOfSols != INFINITE_ROOTS)
        fprintf(stream, " }\n");

    // FIXME: always close file
    if (outputFile != NULL)
        fclose(stream);
    return QUAD_EQ_ERRORS_OK;
}

QuadEqErrors solveAndPrintEquation(const struct QuadraticEquation* eq, const char* outputFile) {
    ///\throw eq should not be NULL
    assert(eq != NULL);

    if (eq == NULL)
        LOG_AND_RETURN(QUAD_EQ_ERRORS_ILLEGAL_ARG);

    QuadEqErrors error = validateEquation(eq);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    struct QuadraticEquationAnswer answer = {};
    error = getSolutions(eq, &answer);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);
    error = printSolutions(&answer, eq->outputPrecision, outputFile);
    if (error != QUAD_EQ_ERRORS_OK)
        LOG_AND_RETURN(error);

    return QUAD_EQ_ERRORS_OK;
}

