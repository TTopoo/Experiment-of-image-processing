#ifndef __HDR_H__
#define __HDR_H__

struct bmphdr {
    char signature[2];//文件标识符,必须为"BM",即0x424D才是Windows位图文件
    int size;//整个BMP文件的大小(以B为单位)
    short reserved[2];//保留，必须为0
    int offset;//从文件头0000h开始到图像像素数据的字节偏移量（以字节Bytes为单位）,
    //以位图的调色板长度根据位图格式不同而变化
    //可以用这个偏移量快速从文件中读取图像数据
    int hdr_size;//INFOHEADER结构体大小
    int width;//图像宽度
    int height;//图像高度
    short nr_planes;//图像数据平面,bmp存储RGB,因此总为1
    short bits_per_pixel;//图像像素位数
    int compress_type;//0:不压缩; 1:RLE8; 2:RLE4;
    int data_size;//4字节对齐的图像数据大小
    int resol_hori;//用像素表示的水平分辨率
    int resol_vert;//用像素表示的垂直分辨率
    int nr_colors;//实际使用的调色板索引数
    int important_color;//重要的调色板索引数,0:所有的都很重要
    char info[1024];
};

struct bmphdr* get_header(char filename[]);

#endif
