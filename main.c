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

char getSignChar(long double koef) {
    return sign(koef) < 0 ? '-' : '+';
}





// -------------------------- QUADRATIC EQUATION CLASS ---------------------------------------



// quadratic equation looks like this: a * x ^ 2 + b * x + c
// where a, b, c are coefficients, some rational numbers
struct QuadraticEquation {
    long double a, b, c;
};

void PrintEquation(struct QuadraticEquation* eq) {
    long double a = eq->a, b = eq->b, c = eq->c;
    char bSign = getSignChar(b);
    char cSign = getSignChar(c);
    b = fabsl(b), c = fabsl(c);
    printf("Your equation is: %.10Lf * x ^ 2 %c %.10Lf * x %c %.10Lf\n", a, bSign, b, cSign, c);
}

void ReadEquation(struct QuadraticEquation* eq) {
    printf("Equation is expected to look like this: A * x ^ 2 + B * x + C, "
           "where A, B, C are some rational coefficients\n");
    eq->a = getCorrectCoef("Print coefficient A: ");
    eq->b = getCorrectCoef("Print coefficient B: ");
    eq->c = getCorrectCoef("Print coefficient C: ");
}

long double GetPointValue(struct QuadraticEquation* eq, long double x) {
    return eq->a * sq(x) + eq->b * x + eq->c;
}

long double GetDiscriminant(struct QuadraticEquation* eq) {
    return sq(eq->b) - 4 * eq->a * eq->c;
}

// returns x coordinat of top of the parabola
long double GetVertX(struct QuadraticEquation* eq) {
    assert(sign(eq->a) != 0);
    return -eq->b / (2 * eq->a);
}

// returns y coordinat of top of the parabola
long double GetVertY(struct QuadraticEquation* eq) {
    assert(eq->a);
    return -GetDiscriminant(eq) / (4 * eq->a);
}

void append(long double arr[], int* len, long double elem) {
    *len = *len + 1;
    arr[*len - 1] = elem;
}

// gets solutions of quadratic equation (without complex numbers)
void GetSolutions(struct QuadraticEquation* eq, long double solutions[], int* len) {
    // case if this is linear equation
    if (sign(eq->a) == 0) {
        if (sign(eq->b) == 0) {
            if (sign(eq->c) == 0) // infinitely many solutions
                *len = 3;
            else // no solutions at all
                *len = 0;
            return;
        }

        long double root = -eq->c / eq->b;
        append(solutions, len, root);
        return;
    }

    long double disc = GetDiscriminant(eq);
    // no solutions
    if (sign(disc) < 0) return;

    long double root_1 = (-eq->b - sqrtl(disc)) / (2 * eq->a);
    long double root_2 = (-eq->b + sqrtl(disc)) / (2 * eq->a);
    append(solutions, len, root_1);

    // we have 2 distinct solutions
    if (sign(disc) != 0)
        append(solutions, len, root_2);
}

void PrintSolutions(struct QuadraticEquation* eq) {
    int len = 0;
    long double solutions[2];
    GetSolutions(eq, solutions, &len);

    if (len == 3) {
        printf("Infinetly many solutions\n");
        return;
    }

    printf("Solutions of equation : { ");
    for (int i = 0; i < len; ++i) {
        printf("%.10Lf", solutions[i]);
        if (i != len - 1)
            printf(", ");
    }
    printf(" }\n");
}

int main() {
    struct QuadraticEquation equation;

    ReadEquation(&equation);
    PrintEquation(&equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise assertion will be raised
    printf("Coordinate X of top of parabola: %.10Lf\n", GetVertX(&equation));
    printf("Coordinate Y of top of parabola: %.10Lf\n", GetVertY(&equation));
    printf("Value at point 5: %.10Lf\n", GetPointValue(&equation, 5));
    PrintSolutions(&equation);

    return 0;
}
