#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

typedef long double ld;

// EPS = epsilon, regulates with what precision we work
const ld EPS = 1e-9;
const int LINE_LEN = 40;
const ld MAX_KOEF_ABS = 1e18;
const char* COEF_TOO_BIG = "Absolute value of coefficient is too big\n";
const char* INCORRECT_NUM_FORM = "That's not a correct number\n";




// ------------------------ HELPER FUNCTIONS ---------------------------------------

// returns sign of variable x, we use it to avoid some precision problems
int sign(ld x) {
    if (x < -EPS) return -1;
    return x > EPS;
}

// returns square of variable x
ld sq(ld x) {
    return x * x;
}

bool isCorrectFormat(char s[], ld* res) {
    int cnt = 0;
    for (int i = 0; i < strlen(s) - 1; ++i) {
        if (s[i] < '0' || '9' < s[i]) {
            if (s[i] == '.') ++cnt;
            bool si = s[i] == '+' || s[i] == '-';
            if ((si && i) || cnt > 1 || (!si && s[i] != '.')) {
                printf("si : %d, i : %d\n", si, i);
                printf("%s", INCORRECT_NUM_FORM);
                return false;
            }
        }
    }

    char* end;
    *res = strtod(s, &end);
    if (sign(fabsl(*res) - MAX_KOEF_ABS) > 0) {
        printf("error: %s", COEF_TOO_BIG);
        return false;
    }
    return true;
}

ld getCorrectCoef(char inputLine[]) {
    ld res = 0;
    char s[LINE_LEN];
    do {
        printf("%s", inputLine);
        fgets(s, sizeof(s), stdin);
        // printf("s : %s", s);
        bool isOk = isCorrectFormat(s, &res);
        if (isOk) {
            return res;
        }
    } while (true);
}

char getSignChar(ld k) {
    return sign(k) < 0 ? '-' : '+';
}





// -------------------------- QUADRATIC EQUATION CLASS ---------------------------------------



// quadratic equation looks like this: a * x ^ 2 + b * x + c
// where a, b, c are coefficients, some rational numbers
struct QuadraticEquation {
    ld a, b, c;
};

void PrintEquation(struct QuadraticEquation* eq) {
    ld a = eq->a, b = eq->b, c = eq->c;
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

ld GetPointValue(struct QuadraticEquation* eq, ld x) {
    return eq->a * sq(x) + eq->b * x + eq->c;
}

ld GetDiscriminant(struct QuadraticEquation* eq) {
    return sq(eq->b) - 4 * eq->a * eq->c;
}

// returns x coordinat of top of the parabola
ld GetVertX(struct QuadraticEquation* eq) {
    assert(sign(eq->a) != 0);
    return -eq->b / (2 * eq->a);
}

// returns y coordinat of top of the parabola
ld GetVertY(struct QuadraticEquation* eq) {
    assert(eq->a);
    return -GetDiscriminant(eq) / (4 * eq->a);
}

void append(ld* res, int* len, ld elem) {
    *len = *len + 1;
    res = realloc(res, sizeof(*res) * (*len));
    res[*len - 1] = elem;
}

// gets solutions of quadratic equation (without complex numbers)
void GetSolutions(struct QuadraticEquation* eq, ld* res, int* len) {
    // case if this is linear equation
    if (sign(eq->a) == 0) {
        if (sign(eq->b) == 0) {
            if (sign(eq->c) == 0) // infinitely many solutions
                *len = 3;
            else // no solutions at all
                *len = 0;
            return;
        }

        ld x = -eq->c / eq->b;
        append(res, len, x);
        return;
    }

    ld D = GetDiscriminant(eq);
    // no solutions
    if (sign(D) < 0) {
        return;
    }

    ld x1 = (-eq->b - sqrtl(D)) / (2 * eq->a);
    ld x2 = (-eq->b + sqrtl(D)) / (2 * eq->a);
    append(res, len, x1);

    // we have 2 distinct solutions
    if (sign(D) != 0) {
        append(res, len, x2);
    }
}

void PrintSolutions(struct QuadraticEquation* eq) {
    int len = 0;
    ld* res = malloc(0);
    GetSolutions(eq, res, &len);

    if (len == 3) {
        printf("Infinetly many solutions\n");
        return;
    }

    printf("Solutions of equation : { ");
    for (int i = 0; i < len; ++i) {
        printf("%.10Lf", res[i]);
        if (i != len - 1)
            printf(", ");
    }
    printf(" }\n");
    free(res);
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
