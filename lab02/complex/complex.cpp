#pragma once

#include <iostream>

class Complex {
    static unsigned comparison_count;

public:
    double real;
    double complex;    

    Complex(double real = 1., double complex = 0.)
        : real(real), complex(complex) {

    }

    /*
    ~Complex() {

    }
    */

    bool operator==(double a) {
        Complex::comparison_count++;
        return this->real == a && this->complex == 0.;
    }

    Complex operator+(double a) const {
        return Complex(this->real + a, this->complex);
    }

    Complex operator-(double a) const {
        return Complex(this->real - a, this->complex);
    }

    Complex operator*(double a) const {
        return Complex(this->real * a, this->complex * a);
    }

    Complex operator/(double a) const {
        return Complex(this->real / a, this->complex / a);
    }

    bool operator==(Complex &other) const {
        Complex::comparison_count++;
        return this->real == other.real && this->complex == other.complex;
    }

    Complex operator+(Complex &other) const {
        return Complex(this->real + other.real, this->complex + other.complex);
    }

    Complex operator-(Complex &other) const {
        return Complex(this->real - other.real, this->complex - other.complex);
    }

    Complex operator*(Complex &other) const {
        return Complex(
            this->real * other.real - this->complex * other.complex,
            this->real * other.complex + other.real * this->complex
        );
    }

    static unsigned int get_comparison_count() {
        return Complex::comparison_count;
    }
};

unsigned Complex::comparison_count = 0;

std::ostream & operator<<(std::ostream & stream, Complex const & num) {
    return stream << num.real << " + " << num.complex << "i";
}