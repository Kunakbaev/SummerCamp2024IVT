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
void printSolutions(const struct QuadraticEquationAnswer answer);
void solveAndPrintEquation(const struct QuadraticEquation* eq);

