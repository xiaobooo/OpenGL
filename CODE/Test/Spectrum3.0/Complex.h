//
//  Complex.h
//  FFT
//
//  Created by boone on 2018/7/17.
//  Copyright © 2018年 boone. All rights reserved.
//

#ifndef Complex_h
#define Complex_h

#ifndef BOOLEAN_VAL
#define BOOLEAN_VAL
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

class Complex
{
public:
    Complex();
    Complex(double re, double im);
    
    Complex operator=(double v);
    Complex operator+(double v);
    Complex operator-(double v);
    Complex operator*(double v);
    Complex operator/(double v);
    Complex operator+=(double v);
    Complex operator-=(double v);
    Complex operator*=(double v);
    Complex operator/=(double v);
    
    Complex operator=(Complex c);
    Complex operator+(Complex c);
    Complex operator-(Complex c);
    Complex operator*(Complex c);
    Complex operator/(Complex c);
    Complex operator+=(Complex c);
    Complex operator-=(Complex c);
    Complex operator*=(Complex c);
    Complex operator/=(Complex c);
    
    BOOL operator==(Complex c);
    BOOL operator!=(Complex c);
    
    double real;
    double imag;
};

#endif /* Complex_h */
