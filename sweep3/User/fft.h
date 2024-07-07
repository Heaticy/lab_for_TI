  #include "math.h"
  #define PI 3.1415926535
void kfft(float pr[], float pi[], int n, int k, float fr[], float fi[]){ 
	int it,m,is,i,j,nv,l0;
    float p,q,s,vr,vi,poddr,poddi;
    for (it=0; it<=n-1; it++)  //将pr[0]和pi[0]循环赋值给fr[]和fi[]
    { 
		m=it; 
		is=0;
		for(i=0; i<=k-1; i++)
        { 
			j=m/2; 
			is=2*is+(m-2*j); 
			m=j;
		}
        fr[it]=pr[is]; 
        fi[it]=pi[is];
    }
    pr[0]=1.0; 
    pi[0]=0.0;
    p=6.283185306/(1.0*n);
    pr[1]=cos(p); //将w=e^-j2pi/n用欧拉公式表示
    pi[1]=-sin(p);

    for (i=2; i<=n-1; i++)  //计算pr[]
    { 
		p=pr[i-1]*pr[1]; 
		q=pi[i-1]*pi[1];
		s=(pr[i-1]+pi[i-1])*(pr[1]+pi[1]);
		pr[i]=p-q; pi[i]=s-p-q;
    }
    for (it=0; it<=n-2; it=it+2)  
    { 
		vr=fr[it]; 
		vi=fi[it];
		fr[it]=vr+fr[it+1]; 
		fi[it]=vi+fi[it+1];
		fr[it+1]=vr-fr[it+1]; 
		fi[it+1]=vi-fi[it+1];
    }
	m=n/2; 
	nv=2;
    for (l0=k-2; l0>=0; l0--) //蝴蝶操作
    { 
		m=m/2; 
		nv=2*nv;
        for (it=0; it<=(m-1)*nv; it=it+nv)
          for (j=0; j<=(nv/2)-1; j++)
            { 
				p=pr[m*j]*fr[it+j+nv/2];
				q=pi[m*j]*fi[it+j+nv/2];
				s=pr[m*j]+pi[m*j];
				s=s*(fr[it+j+nv/2]+fi[it+j+nv/2]);
				poddr=p-q; 
				poddi=s-p-q;
				fr[it+j+nv/2]=fr[it+j]-poddr;
				fi[it+j+nv/2]=fi[it+j]-poddi;
				fr[it+j]=fr[it+j]+poddr;
				fi[it+j]=fi[it+j]+poddi;
            }
    }
    for (i=0; i<=n-1; i++)
       { 
		  pr[i]=sqrt(fr[i]*fr[i]+fi[i]*fi[i]);  //幅值计算
       }
    return;
}






// int main()
// { 
// 	int i,j;
//     float pr[64],pi[64],fr[64],fi[64],t[64];
//     for (i=0; i<=63; i++)  //生成输入信号
//     { 
// 		t[i] = i*0.001;
// 		pr[i]=1.2+2.7*cos(2*PI*33*t[i])+5*cos(2*PI*200*t[i]+PI/2); pi[i]=0.0;
// 	}
		
//     kfft(pr,pi,64,6,fr,fi);  //调用FFT函数
// 	for (i=0; i<64; i++)
//     { 
//         printf("%d\t%lf\n",i,pr[i]); //输出结果
//     }
// }
