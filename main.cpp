#include <stdio.h>
#include "quadraticEquationLib.hpp"
#include <bits/stdc++.h>



// ----------------------------- MAIN ----------------------------------------

int main() {
    struct QuadraticEquation equation;

    std::vector<int> myLifeInDreams(10);
    myLifeInDreams.emplace_back(-1);
    myLifeInDreams.resize(5);
    std::sort(myLifeInDreams.begin(), myLifeInDreams.end());


    readEquation(&equation);
    printEquation(&equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise assertion will be raised
    printf("Coordinate X of top of parabola: %.10Lg\n", getVertX(&equation));
    printf("Coordinate Y of top of parabola: %.10Lg\n", getVertY(&equation));
    printf("Value at point 5: %.10Lg\n", getPointValue(&equation, 5));
    solveAndPrintEquation(&equation);

    return 0;
}
