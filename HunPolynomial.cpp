#include "HunPolynomial.h"

Term::Term(){
    coefficient = 0;
    exponent = 0;
}

Term::Term(int newCoefficient, int newExponent){
    coefficient = newCoefficient;
    exponent = newExponent;
}

int Term::getCoefficient(){
    return coefficient;
}
int Term::getExponent(){
    return exponent;
}

Term Term::operator+(Term term){//must have the same exponent
    Term result;
    if (exponent == term.exponent){
        result.exponent = term.exponent;
        result.coefficient = coefficient + term.coefficient;
    }
    return result;
}

Term Term::operator-(Term term){
    Term result;
    if (exponent == 0 && coefficient == 0){
        result.coefficient = 0 - term.coefficient;
        result.exponent = term.exponent;
    }
    else if (exponent == term.exponent){
        result.exponent = term.exponent;
        result.coefficient = coefficient - term.coefficient;
    }
    return result;
}

Term Term::operator*(Term term){
    Term result;
    result.coefficient = this->coefficient * term.coefficient;
    result.exponent = this->exponent + term.exponent;
    return result;
}


std::ostream& operator<<(std::ostream& os, const Term& term){
    //operator for printing. read content into outsream
    if (term.coefficient == 1){
        if (term.exponent == 0)
            os << term.coefficient;
        else if (term.exponent == 1)
            os << "x";
        else
            os << "x^" << term.exponent;
    }
    else if (term.coefficient == -1){
        if (term.exponent == 0)
            os << term.coefficient;
        else if (term.exponent == 1)
            os << "- "<<"x";
        else
            os << "- " << "x^" << term.exponent;
    }
    else if (term.coefficient < 0){
        if (term.exponent == 0)//need abs in order to put space in between - and number
            os << "- " << abs(term.coefficient);
        else if(term.exponent == 1)
            os << "- " << abs(term.coefficient) << "*x";
        else
            os << "- " << abs(term.coefficient) << "*x^" << term.exponent;
    }
    else if (term.coefficient > 0){
        if (term.exponent == 0)
            os << term.coefficient;
        else if(term.exponent == 1)
            os << term.coefficient << "*x";
        else
            os << term.coefficient << "*x^" << term.exponent;
    }
    return os;
}


HunPolynomial::HunPolynomial(){//default constructor creates empty polynomial
    std::vector<Term> terms;    //initialized with empty vector, with 0 terms. might be unneccesary
}

HunPolynomial::HunPolynomial(std::vector<int> nums){//parameterized constructor
    terms.clear();
    for (int i = 0, j = nums.size()-1; i < nums.size(); i++, j--) {
        Term newTerm(nums[i],j);
        terms.push_back(newTerm);
    }
}

void HunPolynomial::Set(std::vector<int> nums){
    terms.clear();
    for (int i = 0, j = nums.size()-1; i < nums.size(); i++, j--) {
        Term newTerm(nums[i],j);
        terms.push_back(newTerm);
    }
}
//adds two polynomials and returns result
HunPolynomial HunPolynomial::operator+(HunPolynomial polynomial){
    HunPolynomial result;
    if (polynomial.terms.empty()){ //two edge cases where one or two polynomials are empty
        result.terms = this->terms;
        return result;
    }
    if (terms.empty()){
        result.terms = polynomial.terms;
        return result;
    }
    int i = 0, j = 0;
    while (i < this->terms.size() || j < polynomial.terms.size()) {
        if (this->terms[i].getExponent() == polynomial.terms[j].getExponent()) {
            result.terms.push_back(this->terms[i] + polynomial.terms[j]);
            i++;
            j++;
        }
        else if (this->terms[i].getExponent() > polynomial.terms[j].getExponent()) {
            result.terms.push_back(this->terms[i]);
            i++;
        }
        else if (this->terms[i].getExponent() < polynomial.terms[j].getExponent()) {
            result.terms.push_back(polynomial.terms[j]);
            j++;
        }
    }
    return result;
}

//subtract polynomial
HunPolynomial HunPolynomial::operator-(HunPolynomial polynomial){
    HunPolynomial result;
    if (polynomial.terms.empty()){ //two edge cases where one or two polynomials are empty
        result.terms = this->terms;
        return result;
    }
    if (terms.empty()) {
        for (int i = 0; i < polynomial.terms.size(); i++){
            Term emptyTerm;
            result.terms.push_back(emptyTerm - polynomial.terms[i]);     
        }
        return result;
    }
    for (int i = 0; i < this->terms.size(); i++){
        result.terms.push_back(this->terms[i]);
    }
    for (int j = 0; j < polynomial.terms.size(); j++){
        Term emptyTerm;
        result.terms.push_back(emptyTerm - polynomial.terms[j]);
    }
    result.simplify();
    return result;
}

//multiply polynomial
HunPolynomial HunPolynomial::operator*(HunPolynomial polynomial){
    HunPolynomial result;
    if (polynomial.terms.empty()){ //two edge cases where one or two polynomials are empty
        return result;
    }
    if (terms.empty()){
        return result;
    }
    for (int i = 0; i < this->terms.size(); i++){
        for (int j = 0; j < polynomial.terms.size(); j++){
            result.terms.push_back(this->terms[i] * polynomial.terms[j]);
        }
    }
    result.simplify();    //combine like terms and put in descending order
    return result;
}

std::ostream& operator<<(std::ostream& os, const HunPolynomial& poly){//operator for printing. read content into outsream
    if (poly.terms.empty()){
        os << "empty polynomial";
        return os;
    }
    os << poly.terms[0];
    for (int i = 1; i < poly.terms.size(); i++){
        //if it is posotive, add a plus, else, add a space because minus symbol is already part of the coefficient
        Term currentTerm = poly.terms[i];
        if (currentTerm.getCoefficient() > 0)
            os << " + " << currentTerm;
        else if(currentTerm.getCoefficient() < 0)
            os << " " << currentTerm;
    }
    return os;
}

float HunPolynomial::operator()(float x0){
    float value;
    for (int i = 0; i < terms.size(); i++){
        Term currentTerm = terms[i];
        value += currentTerm.getCoefficient() * pow(x0, currentTerm.getExponent());
    }
    return value;
}

void HunPolynomial::simplify(){    //simplify and sort by exponent
    HunPolynomial temp;
    int i = 0;
    while (i < this->terms.size()){
        int j = i + 1;
        while (j < terms.size()){
            if (terms[i].getExponent() != terms[j].getExponent()){
                j++;
            }
            else if(terms[i].getExponent() == terms[j].getExponent()){
                terms[i] = terms[i] + terms[j];
                terms.erase(terms.begin()+j);
            }
        }
        i++; 
    }
    //sort by exponent using bubble sort
    for (int i = 0; i < terms.size() - 1; i++){
        for (int j = 0; j < terms.size() - i -1; j++){
            if(terms[j].getExponent() < terms[j+1].getExponent())
                std::swap(terms[j], terms[j+1]);
        }
    }
}
