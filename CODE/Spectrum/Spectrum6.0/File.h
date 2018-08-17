//
//  File.h
//  Spectrum6.0
//
//  Created by boone on 2018/8/17.
//  Copyright © 2018年 boone. All rights reserved.
//
#include <iostream>
#include<string>
#include <vector>
#include <cmath>
#include <unistd.h>

#ifndef File_h
#define File_h

struct wav_struct
{
    unsigned long file_size;        //文件大小
    unsigned short channel;            //通道数
    unsigned long frequency;        //采样频率
    unsigned long Bps;                //Byte率
    unsigned short sample_num_bit;    //一个样本的位数
    unsigned long data_size;        //数据大小
    unsigned char *data;            //音频数据
};

class File{
public:
    File();
    File(std::string filepath);
    void fileParse();
    
public:
    std::string path;
    std::vector<float> vertices;
    wav_struct WAV;
    
};

#endif /* File_h */
