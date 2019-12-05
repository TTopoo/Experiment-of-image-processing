#include <stdio.h>
int Median(int n1,int n2,int n3,
		int n4,int n5,int n6,
		int n7,int n8,int n9){
	int i,j,arr[9],flag,temp;
	arr[0] = n1;	arr[1] = n2;	arr[2] = n3;
	arr[3] = n4;	arr[4] = n5;	arr[5] = n6;
	arr[6] = n7;	arr[7] = n8;	arr[8] = n9;
	for(i = 0;i < 9;++i){
		flag=0;
		for(j = 0;j < 9;++j){
			if(arr[j]>arr[j+1]){
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
				flag=1;
			}
		}
		if(flag==0){
			break;	
		}
	}
	return arr[4];
}
int main()
{
    int i, j, k1, k2, k3;
	const int w=4,h=4;
    int in[] = { 1,3,5,2,
				5,6,255,4,
				5,6,8,2,
				3,4,6,2 };
	int out[w*h];
    for(i = 0;i < h ; ++i){		//高
		for(j = 0;j < w ; ++j){		//宽
			
			k1 = (i - 1) * h + j;	//上一行
			k2 = i * h + j;		//这一行
			k3 = (i + 1) * h + j;	//下一行
				
			if(	(i - 1) >= 0 && (i + 1) < h && 
				(j - 1) >= 0 && (j + 1) < w ){
				
				out[k2] = Median(in[k1 - 1], in[k1], in[k1 + 1],
								in[k2 - 1], in[k2], in[k2 + 1],	
								in[k3 - 1], in[k3], in[k3 + 1]);
			}
			else {
				k2 = i * w + j;
				out[k2] = in[k2];
			}
		}
	}
	
	for(i=0;i<h;++i){
		for(j=0;j<w;++j){
			printf("%d ",in[i*w+j]);
		}
		printf("\n");
	}
	printf("\n");
	for(i=0;i<h;++i){
		for(j=0;j<w;++j){
			printf("%d ",out[i*w+j]);
		}
		printf("\n");
	}

    return 0;

}
