//
//  FFT.h
//  FFT
//
//  Created by boone on 2018/7/17.
//  Copyright © 2018年 boone. All rights reserved.
//

#ifndef FFT_h
#define FFT_h

#include "Complex.h"

#ifndef CONST_PI
#define CONST_PI
const double PI = 3.14159265358979;
const double PI_X2 = 2 * PI;
#endif

/* r=log2(N) */
extern void FFT(Complex *TD, Complex *FD, int r);


#endif /* FFT_h */
