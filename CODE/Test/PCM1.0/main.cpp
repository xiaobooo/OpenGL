//
//  main.cpp
//  PCM1.0
//
//  Created by boone on 2018/7/6.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/16k.pcm"
//#define VOL_FILE_PATH "/Users/boone/Desktop/Music/vol.txt"

#include <iostream>

using namespace std;

int total = 0;

int volume_adjust(short  * in_buf, short  * out_buf, float in_vol)
{
    
    cout<<"原始数值："<<*in_buf<<"   ";
    
    int i, tmp;
    
    // in_vol[0, 100]
    float vol = in_vol - 98;
    
    if(-98<vol && vol<0)
        vol = 1/(vol*(-1));
    else if(0<=vol && vol<=1)
        vol = 1;
    /*
     else if(1<=vol && vol<=2)
     vol = vol;
     */
    else if(vol<=-98)
        vol = 0;
    else if(vol>=2)
        vol = 40;  //这个值可以根据你的实际情况去调整
    
    tmp = (*in_buf)*vol; // 上面所有关于vol的判断，其实都是为了此处*in_buf乘以一个倍数，你可以根据自己的需要去修改
    
    // 下面的code主要是为了溢出判断
    if(tmp > 32767)
        tmp = 32767;
    else if(tmp < -32768)
        tmp = -32768;
    *out_buf = tmp;
    
    cout<<"处理后数据："<<(float)*out_buf/10000<<"当前长度："<<++total<<endl;
    
    
    
    return 0;
}

void pcm_volume_control(int volume)
{
    short s16In = 0;
    short s16Out = 0;
    int size = 0;
    
    FILE *fp = fopen(OLD_FILE_PATH, "rb+");
   // FILE *fp_vol = fopen(VOL_FILE_PATH, "wb+");
    
    while(!feof(fp))
    {
        size = fread(&s16In, 2, 1, fp);
        if(size>0)
        {
            volume_adjust(&s16In, &s16Out, volume);
 //           fwrite(&s16Out, 2, 1, fp_vol);
        }
    }
    
    fclose(fp);
 //   fclose(fp_vol);
}

int main(void)
{
    pcm_volume_control(100);
    return 0;
}
