#include <stdio.h>
#include <memory.h>
#include "hdr.h"
struct bmphdr* get_header(char filename[])
{
    FILE* fp;
    struct bmphdr* hdr;
    fp = fopen(filename, "rb");
    if (!fp) {
        printf("File open error or such file does not exist!\n");
        return NULL;
    }
    hdr = (struct bmphdr*) malloc(sizeof(struct bmphdr));
    fread(hdr->signature, 2, 1, fp);
    if (hdr->signature[0] != 'B' || hdr->signature[1] != 'M') {
        printf("Not a bmp file!\n");
        return NULL;
    }
    fread(&hdr->size, 4, 1, fp);
    fread(hdr->reserved, 4, 1, fp);
    fread(&hdr->offset, 4, 1, fp);
    fread(&hdr->hdr_size, 4, 1, fp);
    fread(&hdr->width, 4, 1, fp);
    fread(&hdr->height, 4, 1, fp);
    fread(&hdr->nr_planes, 2, 1, fp);
    fread(&hdr->bits_per_pixel, 2, 1, fp);
    fread(&hdr->compress_type, 4, 1, fp);
    fread(&hdr->data_size, 4, 1, fp);
    fread(&hdr->resol_hori, 4, 1, fp);
    fread(&hdr->resol_vert, 4, 1, fp);
    fread(&hdr->nr_colors, 4, 1, fp);
    fread(&hdr->important_color, 4, 1, fp);
    if (hdr->offset > 54)
        fread(&hdr->info, 1024, 1, fp);
    fclose(fp);
    return hdr;
}


