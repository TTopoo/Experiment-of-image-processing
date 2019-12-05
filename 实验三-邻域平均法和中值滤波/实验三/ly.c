#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "hdr.h"
typedef unsigned char uchar;
struct bmphdr* hdr;
unsigned char* bitmap,* to;
int count[256],acum[256];
char buf[2048];

uchar AvgBox(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
	uchar n6, uchar n7, uchar n8, uchar n9){
		return (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9)/9.0;
}
uchar AvgBox(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
	uchar n6, uchar n7, uchar n8, uchar n9, uchar n10,
	uchar n11, uchar n12, uchar n13, uchar n14, uchar n15,
	uchar n16, uchar n17, uchar n18, uchar n19, uchar n20,
	uchar n21, uchar n22, uchar n23, uchar n24, uchar n25) {//返回均值

	uchar avg;
	avg = ( n1 + n2 + n3 + n4 + n5 + 
			n6 + n7 + n8 + n9 + n10 + 
			n11 + n12 + n13 + n14 + n15 + 
			n16 + n17 + n18 + n19 + n20 + 
			n21 + n22 + n23 + n24 + n25 )/25.0;
	
	return avg;
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
			
			k0 = (i - 2) * hdr->width + j;	//上上一行
			k1 = (i - 1) * hdr->width + j;	//上一行
			k2 = i * hdr->width + j;		//这一行
			k3 = (i + 1) * hdr->width + j;	//下一行
			k4 = (i + 2) * hdr->width + j;	//下下一行
			
			if(	(i - 2) >= 0 && (i + 2) <= hdr->width && 
				(j - 2) >= 0 && (j + 2) <= hdr->height ){
				/*
				to[k2] = AvgBox(bitmap[k0 - 2], bitmap[k0 - 1], bitmap[k0], bitmap[k0 + 1],bitmap[k0 + 2],
									bitmap[k1 - 2], bitmap[k1 - 1], bitmap[k1], bitmap[k0 + 1],bitmap[k1 + 2],
									bitmap[k2 - 2], bitmap[k2 - 1], bitmap[k2], bitmap[k2 + 1],bitmap[k2 + 2],
									bitmap[k3 - 2], bitmap[k3 - 1], bitmap[k3], bitmap[k3 + 1],bitmap[k3 + 2],
									bitmap[k4 - 2], bitmap[k4 - 1], bitmap[k4], bitmap[k4 + 1],bitmap[k4 + 2],
									);*/
				to[k2] = AvgBox(bitmap[k1 - 1], bitmap[k1], bitmap[k0 + 1],
								bitmap[k2 - 1], bitmap[k2], bitmap[k2 + 1],
								bitmap[k3 - 1], bitmap[k3], bitmap[k3 + 1],
								);
			}
			else {
				k2 = i * hdr->width + j;
				to[k2] = bitmap[k2];
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
    fwrite(to, nr_pixels, 1, fpnew);//写图像
    //say goodbye
    fclose(fpnew);
    free(hdr);
    free(bitmap);
	free(to);
    return 0;
}
