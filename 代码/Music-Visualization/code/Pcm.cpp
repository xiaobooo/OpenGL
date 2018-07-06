//
//  main.cpp
//  PCM2.0
//
//  Created by boone on 2018/7/6.
//  Copyright © 2018年 boone. All rights reserved.
//


#define OLD_FILE_PATH "/Users/boone/Desktop/Music/16k.pcm"     //PCM源文件
#define NEW_FILE_PATH "/Users/boone/Desktop/Music/new.pcm"     //PCM解析后数据输出

#include <iostream>

using namespace std;

int volume_output(short  * in_buf, short  * out_buf)
{
    
    cout<<"原始数值："<<*in_buf<<endl;
    
    int temp;
    
    temp = *in_buf; // 接受来自PCM文件中的音量数据
    
    //temp*=10;       //此行可用来调整获取的音量数据
    
    // 下面的code主要是为了溢出判断，当数据溢出时取最大值或最小值
    if(temp > 32767)
        temp = 32767;
    else if(temp < -32768)
        temp = -32768;
    
    *out_buf = temp;   //输出解码PCM得到的数据
    
    cout<<"获取的数值："<<*out_buf<<endl;
    
    return 0;
}

void pcm_read_write()
{
    short pcm_In = 0;
    short pcm_Out = 0;
    int size = 0;
    
    FILE *fp = fopen(OLD_FILE_PATH, "rb+");     //为读写打开一个二进制文件 即pcm文件
    FILE *fp_new = fopen(NEW_FILE_PATH, "wb+");  //以二进制方式进行读写，可以对获取的数据写入一个新的pcm文件
    
    while(!feof(fp))
    {
        size = fread(&pcm_In, 2, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if(size>0)
        {
            volume_output(&pcm_In, &pcm_Out);    //从PCM中获取的数据经过处理后输出
            
            fwrite(&pcm_Out, 2, 1, fp_new);     //以同样的方式写入文件
        }
    }
    
    fclose(fp);
    
    fclose(fp_new);
}

int main(void)
{
    pcm_read_write();
    return 0;
}
