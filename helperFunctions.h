


// EPS = epsilon, regulates with what precision we work
const long double EPS = 1e-9;
const int LINE_LEN = 40;
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

static bool isCorrectFormat(char line[], long double* koef) {
    errno = 0;
    char* endPtr;
    *koef = strtod(line, &endPtr);
    return errno == 0 && *endPtr == '\0' && fabsl(*koef) < MAX_KOEF_ABS;
}

/*

Не у всех функций есть прототип (declaration), не нравится char line[LINE_LEN], хочет чтобы вместо LINE_LEN было просто какое-то число (LINE_LEN это и так константа),

*/

static long double getCorrectCoef(char inputLine[]) {
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


