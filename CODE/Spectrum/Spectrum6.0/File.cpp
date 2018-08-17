//
//  File.cpp
//  Spectrum6.0
//
//  Created by boone on 2018/8/17.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "File.h"

using namespace std;

File::File(string filepath)
{
    this->path=filepath;
}

void File::fileParse()
{
    fstream fs;
    
    fs.open(path, ios::binary | ios::in);
    
    fs.seekg(0x28);
    fs.read((char*)&WAV.data_size, sizeof(WAV.data_size));
    
    WAV.data = new unsigned char[WAV.data_size];
    
    fs.seekg(0x2c);
    fs.read((char *)WAV.data, sizeof(char)*WAV.data_size);
    
    int n=WAV.data_size;
    
    for (unsigned long i =0; i<WAV.data_size; i = i + 2)
    {
        //右边为大端
        unsigned long data_low = WAV.data[i];
        unsigned long data_high = WAV.data[i + 1];
        double data_true = data_high * 256 + data_low;
        long data_complement = 0;
        //取大端的最高位（符号位）
        int my_sign = (int)(data_high / 128);
        
        if (my_sign == 1)
        {
            data_complement = data_true - 65536;
        }
        else
        {
            data_complement = data_true;
        }
        
        double float_data = (double)(data_complement/(double)32768);
        //printf("%f ", float_data);
        vertices.push_back(float_data);
        
    }
    fs.close();
    
    delete[] WAV.data;
}
