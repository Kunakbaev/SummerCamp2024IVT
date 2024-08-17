#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

// EPS = epsilon, regulates with what precision we work
const long double EPS = 1e-9;
const int LINE_LEN = 40;
const long double MAX_KOEF_ABS = 1e18;
// const char* COEF_TOO_BIG = "Error: absolute value of coefficient is too big\n";
const char* INCORRECT_NUM_FORM = "Error: that's not a correct number\n";
const char* LINEAR_EQ_ERROR = "Error: this function can not be used with a linear equation\n";



// ------------------------ HELPER FUNCTIONS ---------------------------------------

// returns sign of variable x, we use it to avoid some precision problems
int sign(long double x) {
    if (x < -EPS) return -1;
    return x > EPS;
}

// returns square of variable x
long double sq(long double x) {
    return x * x;
}

bool isCorrectFormat(char line[], long double* koef) {
    errno = 0;
    char* endPtr;
    *koef = strtod(line, &endPtr);
    return errno == 0 && *endPtr == '\0' && fabsl(*koef) < MAX_KOEF_ABS;
}

long double getCorrectCoef(char inputLine[]) {
    long double koef = 0;
    char line[LINE_LEN];
    do {
        printf("%s", inputLine);
        scanf("%s", line);
        if (isCorrectFormat(line, &koef))
            return koef;
        fprintf(stderr, "%s", INCORRECT_NUM_FORM);
    } while (true);
}




// -------------------------- QUADRATIC EQUATION "CLASS" ---------------------------------------


// quadratic equation looks like this: a * x ^ 2 + b * x + c
// where a, b, c are coefficients, some rational numbers

struct QuadraticEquation {
    long double a, b, c;
};

enum QuadEqRootState {
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    INFINITE_ROOTS,
};

struct QuadraticEquationAnswer {
    long double root_1;
    long double root_2;
    enum QuadEqRootState numOfSols;
};

// structure "methods"

void readEquation(struct QuadraticEquation* eq);
void printEquation(const struct QuadraticEquation* eq);
long double getPointValue(const struct QuadraticEquation* eq, long double x);
long double getDiscriminant(const struct QuadraticEquation* eq);
long double getVertX(const struct QuadraticEquation* eq);
long double getVertY(const struct QuadraticEquation* eq);
void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer);
void printSolutions(struct QuadraticEquationAnswer answer);
void solveAndPrintEquation(struct QuadraticEquation* eq);



// ----------------------------- MAIN ----------------------------------------

int main() {
    struct QuadraticEquation equation;

    readEquation(&equation);
    printEquation(&equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise assertion will be raised
    printf("Coordinate X of top of parabola: %.10Lg\n", getVertX(&equation));
    printf("Coordinate Y of top of parabola: %.10Lg\n", getVertY(&equation));
    printf("Value at point 5: %.10Lg\n", getPointValue(&equation, 5));
    solveAndPrintEquation(&equation);

    return 0;
}




char getSignChar(long double koef) {
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

void solveLinearEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    if (sign(eq->b) == 0) {
        answer->numOfSols = sign(eq->c) ? NO_ROOTS : INFINITE_ROOTS;
        return;
    }

    answer->root_1 = -eq->c / eq->b;
    answer->numOfSols = ONE_ROOT;
}

void solveQuadraticEquation(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer) {
    long double disc = getDiscriminant(eq);
    // negative disc -> no solutions
    if (sign(disc) < 0) {
        answer->numOfSols = NO_ROOTS;
        return;
    }

    answer->root_1 = (-eq->b - sqrtl(disc)) / (2 * eq->a);
    // we have 2 distinct solutions in case if disc != 0
    answer->numOfSols = ONE_ROOT;
    if (sign(disc) != 0) {
        answer->root_2 = (-eq->b + sqrtl(disc)) / (2 * eq->a);
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

void printSolutions(struct QuadraticEquationAnswer answer) {
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

void solveAndPrintEquation(struct QuadraticEquation* eq) {
    struct QuadraticEquationAnswer answer;
    getSolutions(eq, &answer);
    printSolutions(answer);
}
