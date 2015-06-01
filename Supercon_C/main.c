/*
 SuperCon 2015 予選問題
 チーム名:未定
 
 最終更新:06/02 01:17
 */


/*標準ライブラリ*/
#include <stdio.h>  //scanf,printf,...
#include <stdlib.h> //qsort
#include <time.h>

/*プロトタイプ宣言*/
int comp_rep(const void * A, const void * B);
int comp_robot(const void * A, const void * B);
int lower_bound_robot(int Q);
int upper_bound_robot_2(int Q);
int upper_bound_robot(int Q,int min_Z);
int get_length_r(int num);
int get_length_l(int num);
int max(int a,int b);
int min(int a,int b);


/*定数*/
const int SIZE = 300; // 300000
const int INF = 1000000000;


/*構造体*/
typedef struct {
    
    int z;
    int q;
    int num;
    
} ROBOT;

typedef struct {
    
    int x;
    int p;
    int num;
    int to_l;
    int to_r;
    int check;
    
} REP;


/*変数宣言*/
int L, n, m;

ROBOT robot[SIZE];
REP rep[SIZE],rep_in[SIZE];

int ans[SIZE],sum[SIZE+1],sum_n[SIZE+1];


/* 時間計測用（デバッグ等に使って下さい） */
clock_t start, end;


/*メイン関数*/
int main(void)
{
    int up,low,l,r,a,b;
    
    /*入力*/
    scanf("%d %d", &L, &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &rep[i].x, &rep[i].p);
        
        rep[i].num = i;
        rep[i].to_l = i-1;
        rep[i].to_r = i+1;
        rep[i].check = 0;
        
        rep_in[i]=rep[i];
    }
    
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &robot[i].z, &robot[i].q);
        
        robot[i].num = i;
    }
    
    start = clock(); //計測
    
    
    /*ソート*/
    qsort((void*)rep,n,sizeof(rep[0]),comp_rep);
    qsort((void*)robot,m,sizeof(robot[0]),comp_robot);

    /*計算*/
    for(int i=0;i<n;i++){
        
        if(rep_in[rep[i].num].check == 0){
            
            l = get_length_l(rep[i].num);
            r = get_length_r(rep[i].num);
            
            a = rep[i].x-l;
            b = rep[i].x+r;
            
            up = upper_bound_robot_2(rep[i].p);
            low = lower_bound_robot(rep[i].p);
            
            if(-1!=low && -1!=up)
                for(int j=low;j<=up;j++){
                    if(a+robot[j].z < b-robot[j].z){
                        sum[j]+=max(0,min(L,min(rep[i].x+robot[j].z,b-robot[j].z))-max(0,max(rep[i].x-robot[j].z,a+robot[j].z)));
                    }
                }
            
            rep_in[rep_in[rep[i].num].to_r].to_l = rep_in[rep[i].num].to_l;
            rep_in[rep_in[rep[i].num].to_l].to_r = rep_in[rep[i].num].to_r;
            
            //[low,up]
            /*
            
            sum[low]+=l+r;
            sum[up_l+1]-=l;
            sum[up_r+1]-=r;
            sum_n[low]+=2;
            sum_n[up_l+1]--;
            sum_n[up_r+1]--;
             */
        }
    }
    
    
    /*累積和*/
    /*
    for(int i=0;i<m-1;i++){
        sum[i+1]+=sum[i];
        sum_n[i+1]+=sum_n[i];
    }
     */
    /*答え計算*/
    for(int i=0;i<m;i++){
        //ans[robot[i].num] = sum[i]-sum_n[i]*robot[i].z;
        ans[robot[i].num] = sum[i];
    }
    
    /*出力*/
    end = clock();
    fprintf(stderr, "%f[s]\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    for(int i=0;i<m;i++){
        printf("%d\n",ans[i]);
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
int lower_bound_robot(int Q){
    
    //[l,r]
    
    if(robot[m-1].q < Q) return -1;
    
    int l=0,r=m-1,mid;
    
    while(l<r){
        mid = (l+r)/2;
        
        if(robot[mid].q < Q){
            l = mid+1;
        }else{
            r = mid;
        }
    }
    
    return l;
}

int upper_bound_robot(int Q,int min_Z){
    
    //[l,r]
    
    int l=0,r=m-1,mid;
    
    while(l<r){
        mid = (l+r+1)/2;
        
        if(robot[mid].q > Q  || (robot[mid].q == Q && robot[mid].z >min_Z) ){
            r = mid-1;
        }else{
            l = mid;
        }
    }
    
    return l;
}

int upper_bound_robot_2(int Q){
    
    //[l,r]
    
    if(robot[0].q > Q) return -1;
    
    int l=0,r=m-1,mid;
    
    while(l<r){
        mid = (l+r+1)/2;
        
        if(robot[mid].q > Q){
            r = mid-1;
        }else{
            l = mid;
        }
    }
    
    return l;
}

/*範囲長計算*/
int get_length_r(int num){
    int ret = 0;
    
    if(rep_in[num].to_r <n){
        return rep_in[rep_in[num].to_r].x - rep_in[num].x;
    }else{
        return INF; //L-rep_in[num].x;
    }
    
    return ret;
}

int get_length_l(int num){
    
    if(0<=rep_in[num].to_l){
        return rep_in[num].x - rep_in[rep_in[num].to_l].x;
    }else{
        return INF; //rep_in[num].x;
    }
}

/*max,min*/

int max(int a,int b){
    if(a>b)
        return a;
    else
        return b;
}

int min(int a,int b){
    if(a<b)
        return a;
    else
        return b;
}
