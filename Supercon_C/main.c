/*
 SuperCon 2015 予選問題
 チーム名:onepoint
 O(nlogn + mlogm + nlogm)
 
 最終更新:06/07 23:37
 */


/*標準ライブラリ*/
#include <stdio.h>  //scanf,printf,...
#include <stdlib.h> //qsort
#include <time.h>

/*プロトタイプ宣言*/
int comp_rep(const void * A, const void * B);
int comp_robot(const void * A, const void * B);
long long lower_bound_robot(int Q,long long min_Z,int t);
long long get_length_r(int num);
long long get_length_l(int num);
long long max(long long a,long long b);
long long min(long long a,long long b);

/*定数*/
const int SIZE = 300000; // 300000
const int INF = 1000000000;
const long long LLINF = 100000000000LL;


/*構造体*/
typedef struct {
    
    long long z;
    int q;
    int num;
    
} ROBOT;

typedef struct {
    
    long long x;
    int p;
    int num;
    int to_l;
    int to_r;
    
} REP;


/*変数宣言*/
int L, n, m;

ROBOT robot[SIZE+10];
REP rep[SIZE+10],rep_in[SIZE+10];

long long ans[SIZE+10],sum[SIZE+10],sum_zn[SIZE+10];


/*メイン関数*/
int main(void)
{
    long long l,r,a,b,min_lr,max_lr,border[6],border_n[6],border_add[6],border_add_zn[6],edge;
    
    /*入力*/
    scanf("%d %d", &L, &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld %d", &rep[i].x, &rep[i].p);
        
        rep[i].num = i;
        rep[i].to_l = i-1;
        rep[i].to_r = i+1;
        
        rep_in[i]=rep[i];
    }
    
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        scanf("%lld %d", &robot[i].z, &robot[i].q);
        
        robot[i].num = i;
    }
    
    robot[m].q=INF;
    robot[m].z=LLINF;
    robot[m+1].q=0;
    robot[m+1].z=0;
    
    /*ソート*/
    qsort((void*)rep,n,sizeof(rep[0]),comp_rep);
    qsort((void*)robot,m+2,sizeof(robot[0]),comp_robot);
    
    /*計算*/
    // ans = sum_zn * z + sum;
    for(int i=0;i<n;i++){
        
        l = get_length_l(rep[i].num);
        r = get_length_r(rep[i].num);
        
        min_lr = min(l,r);
        max_lr = max(l,r);
        
        a = rep[i].x-l;
        b = rep[i].x+r;
        
        /*範囲加算*/
        border[0] = 0;
        border[1] = min_lr;
        border[2] = min(2*min_lr,max_lr);
        border[3] = max(2*min_lr,max_lr);
        border[4] = l+r;
        border[5] = LLINF*2;
        
        border_add[0] = 0;
        border_add_zn[0] = 2;
        border_add[1] = min_lr;
        border_add_zn[1] = 0;
        if(2*min_lr < max_lr){
            border_add[2] = min_lr;
            border_add_zn[2] = 0;
        }else{
            border_add[2] = l+r;
            border_add_zn[2] = -2;
        }
        border_add[3] = l+r;
        border_add_zn[3] = -2;
        border_add[4] = 0;
        border_add_zn[4] = 0;
        
        for(int j=0;j<6;j++){
            border_n[j] = lower_bound_robot(rep[i].p, border[j],2);
        }
        
        for(int j=0;j<5;j++){
            sum[border_n[j]]+=border_add[j];
            sum[border_n[j+1]]-=border_add[j];
            sum_zn[border_n[j]]+=border_add_zn[j];
            sum_zn[border_n[j+1]]-=border_add_zn[j];
        }
        
        /*端の処理*/
        if(rep_in[rep[i].num].to_l < 0 && rep_in[rep[i].num].to_r >= n){
            
            edge = lower_bound_robot(rep[i].p, rep[i].x,1);
            sum[edge]+= rep[i].x;
            sum[border_n[5]]-= rep[i].x;
            sum_zn[edge]+= -1;
            sum_zn[border_n[5]]-= -1;
            
            edge = lower_bound_robot(rep[i].p, L-rep[i].x,1);
            sum[edge]+= L-rep[i].x;
            sum[border_n[5]]-= L-rep[i].x;
            sum_zn[edge]+= -1;
            sum_zn[border_n[5]]-= -1;
            
        }else if(rep_in[rep[i].num].to_l < 0){
            
            edge = lower_bound_robot(rep[i].p, rep[i].x,1);
            sum[edge]+= rep[i].x;
            sum[border_n[5]]-= rep[i].x;
            sum_zn[edge]+= -1;
            sum_zn[border_n[5]]-= -1;
            
            edge = lower_bound_robot(rep[i].p,b,1);
            sum[edge]+= -b;
            sum[border_n[5]]-= -b;
            sum_zn[edge]+= 1;
            sum_zn[border_n[5]]-= 1;
            
        }else if(rep_in[rep[i].num].to_r >= n){
            
            edge = lower_bound_robot(rep[i].p, L-rep[i].x,1);
            sum[edge]+= L-rep[i].x;
            sum[border_n[5]]-= L-rep[i].x;
            sum_zn[edge]+= -1;
            sum_zn[border_n[5]]-= -1;
            
            edge = lower_bound_robot(rep[i].p, L-a,1);
            sum[edge]+= a-L;
            sum[border_n[5]]-= a-L;
            sum_zn[edge]+= 1;
            sum_zn[border_n[5]]-= 1;
            
        }
        
        /*隣のリンク*/
        if(n>rep_in[rep[i].num].to_r)
            rep_in[rep_in[rep[i].num].to_r].to_l = rep_in[rep[i].num].to_l;
        if(0<=rep_in[rep[i].num].to_l)
            rep_in[rep_in[rep[i].num].to_l].to_r = rep_in[rep[i].num].to_r;
    }
    
    
    /*累積和*/
    for(int i=0;i<=m;i++){
        sum[i+1]+=sum[i];
        sum_zn[i+1]+=sum_zn[i];
    }
    
    /*答え計算*/
    for(int i=0;i<=m;i++){
        ans[robot[i].num] = sum[i]+sum_zn[i]*robot[i].z;
    }
    
    for(int i=0;i<m;i++){
        printf("%lld\n",ans[i]);
    }
    
    return 0;
}


/*qsort 比較関数*/
int comp_rep(const void * A, const void * B){
    REP A_ = *(REP*)A;
    REP B_ = *(REP*)B;
    
    if(A_.p < B_.p)
        return -1;
    else if(A_.p > B_.p)
        return 1;
    
    if(A_.x < B_.x)
        return -1;
    else if(A_.x > B_.x)
        return 1;
    else
        return 0;
}

int comp_robot(const void * A, const void * B){
    ROBOT A_ = *(ROBOT*)A;
    ROBOT B_ = *(ROBOT*)B;
    
    if(A_.q < B_.q)
        return -1;
    else if(A_.q > B_.q)
        return 1;
    
    if(A_.z < B_.z)
        return -1;
    else if(A_.z > B_.z)
        return 1;
    else
        return 0;
}

/*二分探索*/
long long lower_bound_robot(int Q,long long min_Z,int t){
    
    //[l,r]
    int l=0,r=m+1,mid;
    
    while(l<r){
        mid = (l+r)/2;
        
        if(robot[mid].q < Q  || (robot[mid].q == Q && robot[mid].z*t < min_Z)){
            l = mid+1;
        }else{
            r = mid;
        }
    }
    
    return (long long)l;
}

/*範囲長計算*/
long long get_length_r(int num){
    
    if(rep_in[num].to_r <n){
        return rep_in[rep_in[num].to_r].x - rep_in[num].x;
    }else{
        return LLINF; //L-rep_in[num].x;
    }
}

long long get_length_l(int num){
    
    if(0<=rep_in[num].to_l){
        return rep_in[num].x - rep_in[rep_in[num].to_l].x;
    }else{
        return LLINF; //rep_in[num].x;
    }
}

/*max,min*/

long long max(long long a,long long b){
    if(a>b)
        return a;
    else
        return b;
}

long long min(long long a,long long b){
    if(a<b)
        return a;
    else
        return b;
}
