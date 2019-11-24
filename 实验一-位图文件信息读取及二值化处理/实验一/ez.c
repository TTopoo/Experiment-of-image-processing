#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "hdr.h"

struct bmphdr* hdr;
unsigned char* bitmap, * to;
char buf[2048];

int main(int argc, char* argv[])//argc是参数个数;argv是具体的每一个参数,可用下标控制
{
    int i, j, k, nr_pixels;
    FILE* fp, * fpnew;
    unsigned g;
    if (argc != 4) {//判断参数个数是否正确
        printf("Usage:%s<file_from><file_to><threshold>\n", argv[0]);
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
    k = atoi(argv[3]);//k是二值化的阈值
    to = malloc(nr_pixels);//再开一个 图像大小的空间
    memset(to, 0, nr_pixels);//空间初始化为0
    for (i = 0; i < nr_pixels; i++)
		//逐位判断每一个像素点值是否大于k
        to[i] = bitmap[i] > (unsigned char)k ? 255 : 0;
	
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
    return 0;
}
