#include<stdio.h>
#include<string.h>

const int MAX=100000;

int flag[MAX];
int imin[MAX];
int L,S,T,M;
int res=10000;

void searchRock(){
    
    for(int i=S;i<=L+T-1;i++)
    {
        for(int j=i-T;j<=i-S;j++)
        {
            if(j>=0&&imin[j]!=-1)//j 点能够跳到
            {
                if(imin[i]==-1)imin[i]=imin[j]+flag[i];
                else if(imin[i]>imin[j]+flag[i]) imin[i]=imin[j]+flag[i];
            }
        }
    }
    
}

int main()
{
    int pos;
    scanf("%d%d%d%d",&L,&S,&T,&M);
    
    memset(flag,0,sizeof(flag));
    memset(imin,-1,sizeof(imin));//初始化
    
    for(int i=0;i<M;i++)
    {
        scanf("%d",&pos);
        flag[pos]=1;//有石子为1，否则为0
    }
    imin[0]=0;
    
    searchRock();
    
    for(int i=L;i<=L+T-1;i++)  //遍历找出最小值
    {
        if(imin[i]!=-1&&imin[i]<res) res=imin[i];
    }
    
    printf("%d\n",res);
    
    return 0;
}
