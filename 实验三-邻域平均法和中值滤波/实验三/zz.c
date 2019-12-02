#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "hdr.h"
typedef unsigned char uchar;
struct bmphdr* hdr;
unsigned char* bitmap;
int count[256],acum[256];
char buf[2048];


uchar Median(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
	uchar n6, uchar n7, uchar n8, uchar n9) {//返回中值
	int i, j, gap;
	int flag = 0;
	
	uchar arr[9],temp;
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
	return arr[4];//返回中值
}

int main(int argc, char* argv[])//argc是参数个数;argv是具体的每一个参数,可用下标控制
{
    int i, j, k1, k2, k3, nr_pixels;
    FILE* fp, * fpnew;
    if (argc != 3) {//判断参数个数是否正确
        printf("Usage:%s<file_from><file_to>\n", argv[0]);
        exit(1);
    }
    hdr = get_header(argv[1]);
    if (!hdr)//判断图片是否存在
        exit(1);
    fp = fopen(argv[1], "rb");//二进制读取图片信息
    if (!fp) {
        printf("File open error!\n");
        exit(1);
    }
    fseek(fp, hdr->offset, SEEK_SET);//跳过offset前的字段，为了之后读图像方便点
    nr_pixels = hdr->width * hdr->height;//计算像素大小
    bitmap = malloc(nr_pixels);//分配像素大小的空间
    fread(bitmap, nr_pixels, 1, fp);//每次读取一个像素大小的对象到bitmap中,读像素次
    fclose(fp);
	
	printf("%d %d\n",hdr->width,hdr->height);
	for(i = 0;i < hdr->height ; ++i){		//高
		for(j = 0;j < hdr->width ; ++j){		//宽
			
			k1 = (i - 1) * hdr->width + j;	//上一行
			k2 = i * hdr->width + j;		//这一行
			k3 = (i + 1) * hdr->width + j;	//下一行
				
			if(	(i - 1) >= 0 && (i + 1) <= hdr->width && 
				(j - 1) >= 0 && (j + 1) <= hdr->height ){
				
				bitmap[k2] = Median(bitmap[k1 - 1], bitmap[k1], bitmap[k1 + 1],
									bitmap[k2 - 1], bitmap[k2], bitmap[k2 + 1],	
									bitmap[k3 - 1], bitmap[k3], bitmap[k3 + 1]);
				
			}
			else {
				k2 = i * hdr->width + j;
				bitmap[k2] = bitmap[k2];
			}
		}
	}
	
    fpnew = fopen(argv[2], "wb+");//新建一个图像指针
    if (!fpnew) {
        printf("File create error!\n");
        exit(1);
    }
    //文件头往里写就完事儿了
    fwrite(hdr->signature, 2, 1, fpnew);
    fwrite(&hdr->size, 4, 1, fpnew);
    fwrite(hdr->reserved, 4, 1, fpnew);
    fwrite(&hdr->offset, 4, 1, fpnew);
    fwrite(&hdr->hdr_size, 4, 1, fpnew);
    fwrite(&hdr->width, 4, 1, fpnew);
    fwrite(&hdr->height, 4, 1, fpnew);
    fwrite(&hdr->nr_planes, 2, 1, fpnew);
    fwrite(&hdr->bits_per_pixel, 2, 1, fpnew);
    fwrite(&hdr->compress_type, 4, 1, fpnew);
    fwrite(&hdr->data_size, 4, 1, fpnew);
    fwrite(&hdr->resol_hori, 4, 1, fpnew);
    fwrite(&hdr->resol_vert, 4, 1, fpnew);
    fwrite(&hdr->nr_colors, 4, 1, fpnew);
    fwrite(&hdr->important_color, 4, 1, fpnew);

    if (hdr->offset > 54)//文件起始位置到图像像素数据的字节偏移量如果大于54
        fwrite(hdr->info, hdr->offset - 54, 1, fpnew);//把info里面的offset-54 大小字节的元素输出到新图中
    fwrite(bitmap, nr_pixels, 1, fpnew);//写图像
    //say goodbye
    fclose(fpnew);
    free(hdr);
    free(bitmap);
    return 0;
}
