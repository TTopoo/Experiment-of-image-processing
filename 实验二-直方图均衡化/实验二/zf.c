#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "hdr.h"

struct bmphdr* hdr;
unsigned char* bitmap;
unsigned char new_color[256];
int count[256],acum[256];
char buf[2048];

int main(int argc, char* argv[])//argc是参数个数;argv是具体的每一个参数,可用下标控制
{
    int i, j, k, nr_pixels;
    FILE* fp, * fpnew;
    unsigned g;
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
    
    memset(count,0,sizeof(count));
	
	for(i = 0;i < nr_pixels;i++)//统计每个灰度值出现的次数
		for(j = 0;j < 256;j++)
			if(bitmap[i] == j)
				count[j]++;
			
	memcpy(acum,count,sizeof(acum));
	for(i = 1;i < 256;i++)
		acum[i] += acum[i-1];//累计次数
		
	for(i = 0;i < 256;i++){
		j = floor(acum[i] * 255 / 65536 + 0.5);
		new_color[i] = j;
	}
	for(i = 0;i < nr_pixels;i++)
		bitmap[i]=new_color[bitmap[i]];
	
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
