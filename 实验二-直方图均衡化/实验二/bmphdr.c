#include<stdio.h>

struct bmphdr {
    char signature[2];
    int size;
    short reserved[2];
    int offset;
    int hdr_size;
    int width;
    int height;
    short nr_planes;
    short bits_per_pixe1;
    int compress_type;
    int data_size;
    int resol_hori;
    int resol_vert;
    int nr_colors;
    int important_color;
}header;

int main(int argc, char* argv[])//argc是参数个数;argv是具体的每一个参数,可用下标控制
{
    FILE* fp;
    if (argc != 2) {
        printf("Usage:%s<filename>\n", argv[0]);
        exit(1);
    }
    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("File open error or such file does not exist!\n");
        exit(1);
    }
    fread(header.signature, 2, 1, fp);
    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        printf("Not a bmp file!\n");
        exit(1);
    }
    fread(&header.size, 4, 1, fp);
    fread(header.reserved, 4, 1, fp);
    fread(&header.offset, 4, 1, fp);
    fread(&header.hdr_size, 4, 1, fp);
    fread(&header.width, 4, 1, fp);
    fread(&header.height, 4, 1, fp);
    fread(&header.nr_planes, 2, 1, fp);
    fread(&header.bits_per_pixe1, 2, 1, fp);
    fread(&header.compress_type, 4, 1, fp);
    fread(&header.data_size, 4, 1, fp);
    fread(&header.resol_hori, 4, 1, fp);
    fread(&header.resol_vert, 4, 1, fp);
    fread(&header.nr_colors, 4, 1, fp);
    fread(&header.important_color, 4, 1, fp);
    fclose(fp);

    printf("signature %c%c\n", header.signature[0], header.signature[1]);
    printf("size %d\n", header.size);
    printf("offset %d\n", header.offset);
    printf("hdr_size %d\n", header.hdr_size);
    printf("width %d\n", header.width);
    printf("height %d\n", header.height);
    printf("nr_planes %d\n", header.nr_planes);
    printf("bits_per_pixe1 %d\n", header.bits_per_pixe1);
    printf("compress_type %d\n", header.compress_type);
    printf("data_size %d\n", header.data_size);
    printf("resol_hori %d\n", header.resol_hori);
    printf("resol_vert %d\n", header.resol_vert);
    printf("nr_colors %d\n", header.nr_colors);
    printf("important_color %d\n", header.important_color);
    printf("\n");
    return 0;
}
