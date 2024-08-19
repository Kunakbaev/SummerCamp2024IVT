// -------------------------- QUADRATIC EQUATION "CLASS" ---------------------------------------


// quadratic equation looks like this: a * x ^ 2 + b * x + c
// where a, b, c are coefficients, some rational numbers

struct QuadraticEquation {
    int outputPrecision = 10;
    long double a = 0, b = 0, c = 0;
};

enum QuadEqRootState {
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    INFINITE_ROOTS,
};

struct QuadraticEquationAnswer {
    long double root_1 = 0;
    long double root_2 = 0;
    enum QuadEqRootState numOfSols = NO_ROOTS;
};

// structure "methods"
void readEquation(struct QuadraticEquation* eq);
void printEquation(const struct QuadraticEquation* eq);
long double getPointValue(const struct QuadraticEquation* eq, long double x);
long double getDiscriminant(const struct QuadraticEquation* eq);
long double getVertX(const struct QuadraticEquation* eq);
long double getVertY(const struct QuadraticEquation* eq);
void getSolutions(const struct QuadraticEquation* eq, struct QuadraticEquationAnswer* answer);
void printSolutions(const struct QuadraticEquationAnswer answer, int outputPrecision);
void solveAndPrintEquation(const struct QuadraticEquation* eq);

