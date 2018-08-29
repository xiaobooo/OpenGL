//
//  main.cpp
//  Spectrum3.0
//  引入FFTW进行测试
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

int main()
{
    int n,m;
    cin>>n>>m;
    int num[n];
    for (int i=0; i<n; i++) {
        cin>>num[i];
    }
    float res =0;
    int sum=0;
    int sum2=0;
    for (int i=0; i<n; i++) {
        if (i<m) {
            sum+=num[i];
        }else{
            sum2+=num[i];
        }
        
    }
    res=(m/n)*sum+(1-m/n)*sum2;
    
    cout<<setprecision(2)<<res<<endl;
    return 0;
}
