#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "quadraticEquationLib.hpp"



// EPS = epsilon, regulates with what precision we work
const long double EPSILON = 1e-9;
const int MAX_INPUT_LINE_LEN = 25;
const long double MAX_KOEF_ABS_VALUE = 1e18;

const char* KOEF_TOO_BIG_ERROR = "Error: absolute value of coefficient is too big\n";
const char* INCORRECT_KOEF_FORMAT_ERROR = "Error: that's not a correct number\n";
const char* LINEAR_EQ_ERROR = "Error: this function can not be used with a linear equation\n";
const char* INPUT_LINE_TOO_LONG_ERROR = "Error: input line is too long\n";


// ------------------------ HELPER FUNCTIONS ---------------------------------------

// returns sign of variable x, we use it to avoid some precision problems
static int sign(long double x) {
    if (x < -EPSILON) return -1;
    return x > EPSILON;
}

// returns square of variable x
static long double square(long double x) {
    return x * x;
}

static bool parseLongDoubleAndCheckValid(const char line[], long double* koef) {
    assert(line != NULL && koef != NULL);
    errno = 0;
    char* endPtr;
    *koef = strtod(line, &endPtr);
    return errno == 0 && *endPtr == '\n';
}

static long double getCorrectCoef(const char messageLine[]) {
    assert(messageLine != NULL);
    long double koef = 0;
    bool isGoodNumber = false;

    do {
        printf("%s", messageLine);

        size_t inputLineLen = 0;
        char line[MAX_INPUT_LINE_LEN];
        do {
            fgets(line, MAX_INPUT_LINE_LEN, stdin);
            inputLineLen += strlen(line);
        } while (line[strlen(line) - 1] != '\n');

        if (inputLineLen - 1 > MAX_INPUT_LINE_LEN) {
            fprintf(stderr, "%s", INPUT_LINE_TOO_LONG_ERROR);
            continue;
        }

        if (parseLongDoubleAndCheckValid(line, &koef)) {
            if (fabsl(koef) > MAX_KOEF_ABS_VALUE)
                fprintf(stderr, "%s", KOEF_TOO_BIG_ERROR);
            else
                isGoodNumber = true;
        } else {
            fprintf(stderr, "%s", INCORRECT_KOEF_FORMAT_ERROR);
        }
    } while (!isGoodNumber);

    return koef;
}





// -------------------- FUNCTIONS REALIZATIONS -------------------------------

static char getSignChar(long double koef) {
    return sign(koef) < 0 ? '-' : '+';
}

void printEquation(const struct QuadraticEquation* eq) {
    assert(eq != NULL);
    long double a = eq->a, b = eq->b, c = eq->c;
    char bSign = getSignChar(b);
    char cSign = getSignChar(c);
    b = fabsl(b), c = fabsl(c);
    printf("Your equation is: %.10Lg * x ^ 2 %c %.10Lg * x %c %.10Lg\n", a, bSign, b, cSign, c);
}

void readEquation(struct QuadraticEquation* eq) {
    assert(eq != NULL);
    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational coefficients\n");
    eq->a = getCorrectCoef("Print coefficient A: ");
    eq->b = getCorrectCoef("Print coefficient B: ");
    eq->c = getCorrectCoef("Print coefficient C: ");
}

long double getPointValue(const struct QuadraticEquation* eq, long double x) {
    assert(eq != NULL);
    return eq->a * square(x) + eq->b * x + eq->c;
}

long double getDiscriminant(const struct QuadraticEquation* eq) {
    assert(eq != NULL);
    return square(eq->b) - 4 * eq->a * eq->c;
}

// returns x coordinat of top of the parabola
long double getVertX(const struct QuadraticEquation* eq) {
    assert(eq != NULL);
    if (sign(eq->a) != 0)
        return -eq->b / (2 * eq->a);
    fprintf(stderr, "%s", LINEAR_EQ_ERROR);
    return 0;
}

// returns y coordinat of top of the parabola
long double getVertY(const struct QuadraticEquation* eq) {
    assert(eq != NULL);
    if (sign(eq->a) != 0)
        return -getDiscriminant(eq) / (4 * eq->a);
    fprintf(stderr, "%s", LINEAR_EQ_ERROR);
    return 0;
}

static void solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    assert(eq != NULL && answer != NULL);
    if (sign(eq->b) == 0) {
        answer->numOfSols = sign(eq->c) ? NO_ROOTS : INFINITE_ROOTS;
        return;
    }

    answer->root_1 = -eq->c / eq->b;
    answer->numOfSols = ONE_ROOT;
}

static void solveQuadraticEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    assert(eq != NULL && answer != NULL);
    long double disc = getDiscriminant(eq);
    // negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return;
    }

    long double discRoot = sqrtl(disc);
    long double denom = 1 / (2 * eq->a);
    answer->root_1 = (-eq->b - discRoot) * denom;
    // we have 2 distinct solutions in case if disc != 0
    if (sign(disc) != 0) {
        answer->root_2 = (-eq->b + discRoot) * denom;
        answer->numOfSols = TWO_ROOTS;
    } else {
        answer->numOfSols = ONE_ROOT;
    }
}

// gets solutions of quadratic equation (without complex numbers)
void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    assert(eq != NULL && answer != NULL);
    if (sign(eq->a) == 0) {
        solveLinearEquation(eq, answer);
        return;
    }

    solveQuadraticEquation(eq, answer);
}

void printSolutions(const struct QuadraticEquationAnswer answer) {
    if (answer.numOfSols == INFINITE_ROOTS) {
        printf("Infinetly many solutions\n");
        return;
    }

    printf("Solutions of equation : { ");
    if (answer.numOfSols != NO_ROOTS)
        printf("%.10Lg", answer.root_1);
    if (answer.numOfSols == TWO_ROOTS)
        printf(", %.10Lg", answer.root_2);
    printf(" }\n");
}

void solveAndPrintEquation(const struct QuadraticEquation* eq) {
    assert(eq != NULL);
    struct QuadraticEquationAnswer answer;
    getSolutions(eq, &answer);
    printSolutions(answer);
}

