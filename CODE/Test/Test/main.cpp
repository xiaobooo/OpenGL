//
//  main.cpp
//  Spectrum3.0
//  引入FFTW进行测试
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//
/*
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std ;
#define eqs 1e-12
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
struct node{
    double x , y ;
}p[1000];
int n ;
double maxx , minx , maxy , miny ;
double f(double j) {
    int i ;
    double x , y ;
    maxx = maxy = -INF ;
    minx = miny = INF ;
    for(i = 0 ; i < n ; i++) {
        x = p[i].x*cos(j) - p[i].y*sin(j) ;
        y = p[i].x*sin(j) + p[i].y*cos(j) ;
        maxx = max(maxx,x) ;
        minx = min(minx,x) ;
        maxy = max(maxy,y) ;
        miny = min(miny,y) ;
    }
    return max( maxx-minx,maxy-miny ) ;
}
int main() {
    int  i ;
    
        scanf("%d", &n) ;
        for(i = 0 ; i < n ; i++) {
            scanf("%lf %lf", &p[i].x, &p[i].y) ;
        }
        double low = 0.0 , mid1 , mid2 , high = PI/2.0;
        while( low + eqs < high ) {
            mid1 = (low + high)/2.0 ;
            mid2 = (mid1 + high) / 2.0 ;
            if( f(mid1) > f(mid2) )
                low = mid1 ;
            else
                high = mid2 ;
        }
    
        low = f(low) ;
    int area = low*low;
//        printf("%.2lf\n", low*low ) ;
    cout<<area<<endl;
    return 0 ;
}
*/

#include<stdio.h>
#include<math.h>

#define N 1000
#define eps 0.000001

double PI = acos(-1.0);

typedef struct
{
    double x ,y;
}NODE;

NODE node[N];

double maxx(double x ,double y)
{
    return x > y ? x : y;
}

double minn(double x ,double y)
{
    return x < y ? x : y;
}

double abss(double x)
{
    return x < 0 ? -x : x;
}

double now(double phi ,int n)
{
    double Max_x = -100000000 ,Min_x = 100000000;
    double Max_y = -100000000 ,Min_y = 100000000;
    for(int i = 1 ;i <= n ;i ++)
    {
        double xx = node[i].x * cos(phi) - node[i].y * sin(phi);
        double yy = node[i].x * sin(phi) + node[i].y * cos(phi);
        Max_x = maxx(xx ,Max_x);
        Max_y = maxx(yy ,Max_y);
        Min_x = minn(Min_x ,xx);
        Min_y = minn(Min_y ,yy);
    }
    return maxx((Max_x - Min_x) ,(Max_y - Min_y));
}

int main ()
{
    int n ,i;
    double area;
    double low ,mid ,mmid ,up ,ans;
    
   
        scanf("%d" ,&n);
        for(i = 1 ;i <= n ;i ++)
            scanf("%lf %lf" ,&node[i].x ,&node[i].y);
        low = 0 ,up = PI / 2.0;
        while(1)
        {
            mid = (low + up) / 2.0;
            mmid = (mid + up) / 2.0;
            double dis1 = now(mid ,n);
            double dis2 = now(mmid ,n);
            if(dis1 < dis2) up = mmid;
            else  low = mid;
            if(abss(dis1 - dis2) < eps)
                break;
            ans = minn(dis1 ,dis2);
        }
    
    area=ans*ans;
    
        printf("%lf\n" ,area);
    
    return 0;
}


