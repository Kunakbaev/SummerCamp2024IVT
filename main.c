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
const char* COEF_TOO_BIG = "Absolute value of coefficient is too big\n";
const char* INCORRECT_NUM_FORM = "That's not a correct number\n";



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
        printf("%s", INCORRECT_NUM_FORM);
    } while (true);
}




// -------------------------- QUADRATIC EQUATION "CLASS" ---------------------------------------


// quadratic equation looks like this: a * x ^ 2 + b * x + c
// where a, b, c are coefficients, some rational numbers

struct QuadraticEquation {
    long double a, b, c;
};

// structure "methods"

void readEquation(struct QuadraticEquation* eq);
void printEquation(struct QuadraticEquation* eq);
long double getPointValue(struct QuadraticEquation* eq, long double x);
long double getDiscriminant(struct QuadraticEquation* eq);
long double getVertX(struct QuadraticEquation* eq);
long double getVertY(struct QuadraticEquation* eq);
void getSolutions(struct QuadraticEquation* eq, long double* root_1, long double* root_2, int* cntSols);
void printSolutions(struct QuadraticEquation* eq);



// ----------------------------- MAIN ----------------------------------------

int main() {
    struct QuadraticEquation equation;

    readEquation(&equation);
    printEquation(&equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise assertion will be raised
    // printf("Coordinate X of top of parabola: %.10Lg\n", getVertX(&equation));
    // printf("Coordinate Y of top of parabola: %.10Lg\n", getVertY(&equation));
    // printf("Value at point 5: %.10Lg\n", getPointValue(&equation, 5));
    printSolutions(&equation);

    return 0;
}




char getSignChar(long double koef) {
    return sign(koef) < 0 ? '-' : '+';
}

void printEquation(struct QuadraticEquation* eq) {
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

long double getPointValue(struct QuadraticEquation* eq, long double x) {
    return eq->a * sq(x) + eq->b * x + eq->c;
}

long double getDiscriminant(struct QuadraticEquation* eq) {
    return sq(eq->b) - 4 * eq->a * eq->c;
}

// returns x coordinat of top of the parabola
long double getVertX(struct QuadraticEquation* eq) {
    assert(sign(eq->a) != 0);
    return -eq->b / (2 * eq->a);
}

// returns y coordinat of top of the parabola
long double getVertY(struct QuadraticEquation* eq) {
    assert(eq->a);
    return -getDiscriminant(eq) / (4 * eq->a);
}

// gets solutions of quadratic equation (without complex numbers)
void getSolutions(struct QuadraticEquation* eq, long double* root_1, long double* root_2, int* cntSols) {
    // case if this is linear equation
    if (sign(eq->a) == 0) {
        if (sign(eq->b) == 0) {
            if (sign(eq->c) == 0) // infinitely many solutions
                *cntSols = -1;
            return;
        }

        *cntSols = 1;
        *root_1 = -eq->c / eq->b;
        return;
    }

    long double disc = getDiscriminant(eq);
    // negative disc -> no solutions
    if (sign(disc) < 0) return;

    *cntSols = 1;
    *root_1 = (-eq->b - sqrtl(disc)) / (2 * eq->a);
    printf("%.10Lg", *root_1);
    // we have 2 distinct solutions in case if disc != 0
    if (sign(disc) != 0) {
        *cntSols = 2;
        *root_2 = (-eq->b + sqrtl(disc)) / (2 * eq->a);
    }
}

void printSolutions(struct QuadraticEquation* eq) {
    int cntSols = 0;
    long double root_1, root_2;
    getSolutions(eq, &root_1, &root_2, &cntSols);

    if (cntSols == -1) {
        printf("Infinetly many solutions\n");
        return;
    }

    printf("Solutions of equation : { ");
    if (cntSols >= 1) {
        printf("%.10Lg", root_1);
    }
    if (cntSols == 2) {
        printf(", %.10Lg", root_2);
    }

    printf(" }\n");
}
