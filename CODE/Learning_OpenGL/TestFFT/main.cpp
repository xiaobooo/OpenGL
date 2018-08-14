//
//  main.cpp
//  TestFFT
//
//  Created by boone on 2018/8/13.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#include <fftw3.h>

using namespace std;

#define N 50

int main()
{
    int i;
    fftw_complex *in,*out;
    fftw_plan p;
    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
    if (in==NULL||out==NULL) {
        cout<<"ERROR: Fail to memory allocation"<<endl;
    }else{
        for (i=0; i<N; i++) {
            in[i][0]=i*1000+50;
            in[i][1]=0;
        }
    }
    p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_cleanup();
    for (i=0; i<N; i++) {
//        cout<<in[i][0]<<"   "<<in[i][1]<<"      ";
//        cout<<out[i][0]<<"   "<<out[i][1]<<endl;
        printf("%f,%f,    %f,%f\n",in[i][0],in[0][1],out[i][0],out[i][1]);
    }
    
    if (in!=NULL) {
        fftw_free(in);
    }
    if (out!=NULL) {
        fftw_free(out);
    }
    return 0;
}
