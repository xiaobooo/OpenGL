//
//  main.cpp
//  Spectrum3.0
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//
#include <iostream>
#include <fstream>
using namespace std;

struct wav_struct
{
    unsigned long file_size;        //文件大小
    unsigned short channel;            //通道数
    unsigned long frequency;        //采样频率
    unsigned long Bps;                //Byte率
    unsigned short sample_num_bit;    //一个样本的位数
    unsigned long data_size;        //数据大小
    unsigned char *data;            //音频数据 ,这里要定义什么就看样本位数了，我这里只是单纯的复制数据
    
};

int main(int argc,char **argv)
{
    fstream fs;
    wav_struct WAV;
    fs.open("/Users/boone/Desktop/Music/Seve.wav",ios::binary|ios::in);
    
    //    fs.seekg(0x04);                //从文件数据中获取文件大小
    //    fs.read((char*)&WAV.file_size,sizeof(WAV.file_size));
    //    WAV.file_size+=8;
    
    fs.seekg(0,ios::end);        //用c++常用方法获得文件大小
    WAV.file_size=fs.tellg();
    
    fs.seekg(0x14);
    fs.read((char*)&WAV.channel,sizeof(WAV.channel));
    
    fs.seekg(0x18);
    fs.read((char*)&WAV.frequency,sizeof(WAV.frequency));
    
    fs.seekg(0x1c);
    fs.read((char*)&WAV.Bps,sizeof(WAV.Bps));
    
    fs.seekg(0x22);
    fs.read((char*)&WAV.sample_num_bit,sizeof(WAV.sample_num_bit));
    
    fs.seekg(0x28);
    fs.read((char*)&WAV.data_size,sizeof(WAV.data_size));
    
    WAV.data=new unsigned char[WAV.data_size];
    
    fs.seekg(0x2c);
    fs.read((char *)WAV.data,sizeof(char)*WAV.data_size);
    
    cout<<"文件大小为  ："<<WAV.file_size<<endl;
    cout<<"音频通道数  ："<<WAV.channel<<endl;
    cout<<"采样频率    ："<<WAV.frequency<<endl;
    cout<<"Byte率      ："<<WAV.Bps<<endl;
    cout<<"样本位数    ："<<WAV.sample_num_bit<<endl;
    cout<<"音频数据大小："<<WAV.data_size<<endl;
    cout<<"最后20个数据："<<endl;
    
    for (unsigned long i=WAV.data_size-20;i<WAV.data_size;i++)
    {
        printf("%x  ",WAV.data[i]);
    }
    fs.close();
    
    delete[] WAV.data;
    system("pause");
    
}
