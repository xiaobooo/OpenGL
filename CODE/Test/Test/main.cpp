//
//  main.cpp
//  Spectrum3.0
//  引入FFTW进行测试
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//
#include <iostream>

using namespace std;

int main()
{
    char aa[][3]={'a','b','c','d','e','f'};
    char (*p)[3]=aa;
    p++;
    cout<<**p<<endl;
}
