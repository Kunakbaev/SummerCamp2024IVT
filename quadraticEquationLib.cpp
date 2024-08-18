#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "quadraticEquationLib.hpp"



// EPS = epsilon, regulates with what precision we work
const long double EPS = 1e-9;
const int LINE_LEN = 20;
const long double MAX_KOEF_ABS = 1e18;
// const char* COEF_TOO_BIG = "Error: absolute value of coefficient is too big\n";
const char* INCORRECT_NUM_FORM = "Error: that's not a correct number\n";
const char* LINEAR_EQ_ERROR = "Error: this function can not be used with a linear equation\n";


// ------------------------ HELPER FUNCTIONS ---------------------------------------

// returns sign of variable x, we use it to avoid some precision problems
static int sign(long double x) {
    if (x < -EPS) return -1;
    return x > EPS;
}

// returns square of variable x
static long double sq(long double x) {
    return x * x;
}

static bool isCorrectFormat(const char line[], long double* koef) {
    errno = 0;
    char* endPtr;
    *koef = strtod(line, &endPtr);
    return errno == 0 && *endPtr == '\n' && fabsl(*koef) < MAX_KOEF_ABS;
}

static long double getCorrectCoef(const char inputLine[]) {
    long double koef = 0;
    char line[LINE_LEN];

    do {
        printf("%s", inputLine);
        fgets(line, LINE_LEN - 1, stdin);
        if (isCorrectFormat(line, &koef))
            return koef;
        fprintf(stderr, "%s", INCORRECT_NUM_FORM);
    } while (true);
}

static char getSignChar(long double koef) {
    return sign(koef) < 0 ? '-' : '+';
}

void printEquation(const struct QuadraticEquation* eq) {
    long double a = eq->a, b = eq->b, c = eq->c;
    char bSign = getSignChar(b);
    char cSign = getSignChar(c);
    b = fabsl(b), c = fabsl(c);
    printf("Your equation is: %.10Lg * x ^ 2 %c %.10Lg * x %c %.10Lg\n", a, bSign, b, cSign, c);
}

void readEquation(struct QuadraticEquation* eq) {
    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational coefficients\n");
    eq->a = getCorrectCoef("Print coefficient A: ");
    eq->b = getCorrectCoef("Print coefficient B: ");
    eq->c = getCorrectCoef("Print coefficient C: ");
}

long double getPointValue(const struct QuadraticEquation* eq, long double x) {
    return eq->a * sq(x) + eq->b * x + eq->c;
}

long double getDiscriminant(const struct QuadraticEquation* eq) {
    return sq(eq->b) - 4 * eq->a * eq->c;
}

// returns x coordinat of top of the parabola
long double getVertX(const struct QuadraticEquation* eq) {
    if (sign(eq->a) != 0)
        return -eq->b / (2 * eq->a);
    fprintf(stderr, "%s", LINEAR_EQ_ERROR);
    return 0;
}

// returns y coordinat of top of the parabola
long double getVertY(const struct QuadraticEquation* eq) {
    if (sign(eq->a) != 0)
        return -getDiscriminant(eq) / (4 * eq->a);
    fprintf(stderr, "%s", LINEAR_EQ_ERROR);
    return 0;
}

static void solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    if (sign(eq->b) == 0) {
        answer->numOfSols = sign(eq->c) ? NO_ROOTS : INFINITE_ROOTS;
        return;
    }

    answer->root_1 = -eq->c / eq->b;
    answer->numOfSols = ONE_ROOT;
}

static void solveQuadraticEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    long double disc = getDiscriminant(eq);
    // negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return;
    }

    long double root = sqrtl(disc);
    long double znam = 1 / (2 * eq->a);
    answer->root_1 = (-eq->b - root) * znam;
    // we have 2 distinct solutions in case if disc != 0
    answer->numOfSols = ONE_ROOT;
    if (sign(disc) != 0) {
        answer->root_2 = (-eq->b + root) * znam;
        answer->numOfSols = TWO_ROOTS;
    }
}

// gets solutions of quadratic equation (without complex numbers)
void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
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
    struct QuadraticEquationAnswer answer;
    getSolutions(eq, &answer);
    printSolutions(answer);
}

