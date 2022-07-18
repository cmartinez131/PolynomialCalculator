#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class Term {
private:
    int coefficient;
    int exponent;
public:
    Term();
    Term(int _coefficient, int _exponent);
    int getCoefficient();
    int getExponent();
    Term operator +(Term t);
    Term operator -(Term t); 
    Term operator *(Term t);
    friend std::ostream& operator<<(std::ostream& os, const Term& te);
};



class HunPolynomial {
private:
    std::vector<Term> terms;
public:
    HunPolynomial();
    HunPolynomial(std::vector<int> nums);
    void Set(std::vector<int> nums);
    HunPolynomial operator +(HunPolynomial polynomial);
    HunPolynomial operator -(HunPolynomial polynomial); 
    HunPolynomial operator *(HunPolynomial polynomial);
    friend std::ostream& operator<<(std::ostream& os, const HunPolynomial& po);
    float operator()(float x0);
    void simplify();    //helper function to simply polynomials and sort in decending order
    //no need for custom destructor, copy-assignment, and copy-constructor
};
