//
//  main.cpp
//  Learning_OpenGL
//
//  Created by boone on 2018/7/30.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <iostream>

#include <fftw3.h>

using namespace std;

#define N 10

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
            in[i][0]=i+1;
            in[i][1]=5;
        }
    }
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_cleanup();
    for (i=0; i<N; i++) {
        cout<<in[i][0]<<"   "<<in[i][1]<<endl;
    }
    cout<<"=================================================="<<endl;
    for (i=0; i<N; i++) {
        cout<<out[i][0]<<"   "<<out[i][1]<<endl;
    }
    if (in!=NULL) {
        fftw_free(in);
    }
    if (out!=NULL) {
        fftw_free(out);
    }
    return 0;
}
