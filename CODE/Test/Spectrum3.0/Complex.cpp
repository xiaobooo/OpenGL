//
//  Complex.cpp
//  FFT
//
//  Created by boone on 2018/7/17.
//  Copyright © 2018年 boone. All rights reserved.
//

#include "Complex.h"

Complex::Complex()
{
    real = 0;
    imag = 0;
}

Complex::Complex(double re, double im)
{
    real = re;
    imag = im;
}

Complex Complex::operator+(double v)
{
    return Complex(real + v, imag);
}

Complex Complex::operator-(double v)
{
    return Complex(real - v, imag);
}

Complex Complex::operator*(double v)
{
    return Complex(real*v, imag*v);
}

Complex Complex::operator/(double v)
{
    return Complex(real / v, imag / v);
}

Complex Complex::operator=(double v)
{
    real = v;
    imag = 0;
    return *this;
}

Complex Complex::operator+=(double v)
{
    real += v;
    return *this;
}

Complex Complex::operator-=(double v)
{
    real -= v;
    return *this;
}

Complex Complex::operator*=(double v)
{
    real *= v;
    imag *= v;
    return *this;
}

Complex Complex::operator/=(double v)
{
    real /= 2;
    imag /= 2;
    return *this;
}

Complex Complex::operator+(Complex c)
{
    return Complex(real + c.real, imag + c.imag);
}

Complex Complex::operator-(Complex c)
{
    return Complex(real - c.real, imag - c.imag);
}

Complex Complex::operator*(Complex c)
{
    double re = real*c.real - imag*c.imag;
    double im = real*c.imag + imag*c.real;
    return Complex(re, im);
}

Complex Complex::operator/(Complex c)
{
    double x = c.real;
    double y = c.imag;
    double f = x*x + y*y;
    double re = (real*x + imag*y) / f;
    double im = (imag*x - real*y) / f;
    return Complex(re, im);
}

Complex Complex::operator=(Complex c)
{
    real = c.real;
    imag = c.imag;
    return *this;
}

Complex Complex::operator+=(Complex c)
{
    real += c.real;
    imag += c.imag;
    return *this;
}

Complex Complex::operator-=(Complex c)
{
    real -= c.real;
    imag -= c.imag;
    return *this;
}

Complex Complex::operator*=(Complex c)
{
    double re = real*c.real - imag*c.imag;
    double im = real*c.imag + imag*c.real;
    real = re;
    imag = im;
    return *this;
}

Complex Complex::operator/=(Complex c)
{
    double x = c.real;
    double y = c.imag;
    double f = x*x + y*y;
    double re = (real*x + imag*y) / f;
    double im = (imag*x - real*y) / f;
    real = re;
    imag = im;
    return *this;
}

BOOL Complex::operator==(Complex c)
{
    return ((real == c.real) && (imag == c.imag));
}

BOOL Complex::operator!=(Complex c)
{
    return ((real != c.real) || (imag != c.imag));
}
