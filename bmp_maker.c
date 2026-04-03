#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>

#define _width 20
#define _height 20
#define _img_size _width*_height
#define _file_size 54+4*_img_size
#pragma pack(1)

typedef struct Pixel
{
 uint8_t red;
 uint8_t green;
 uint8_t blue;
 uint8_t reserved;
} Pixel;


typedef struct Header
{
    uint8_t signature[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
} Header;

typedef struct InfoHeader
{
  uint32_t size;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bitCount;
  uint32_t Compression;
  uint32_t imageSize;
  uint32_t xPixelsPerM;
  uint32_t yPixelsPerM;
  uint32_t colorsUsed;
  uint32_t colorsImportant;
} InfoHeader;

void SaveFile(void* buffer,int bufferSize,char* fileName);
int main()
{
    uint8_t* buffer=(void*)malloc(_file_size);
    Header* h=(Header*)&buffer[0];
    InfoHeader* ih= (InfoHeader*)&buffer[14];
    Pixel* colourTable=(Pixel*)&buffer[54];
    /////////////////////////////////////
    (h->signature)[0]='B';
    (h->signature)[1]='M';
    (h->file_size)=_file_size;
    (h->data_offset)=sizeof(Header)+sizeof(InfoHeader); //placeholder

    (ih->size)= sizeof(InfoHeader);
    (ih->width)=_width;
    (ih->height)=_height;
    (ih->planes)=1;
    (ih->bitCount)=24; //bits per pixel
    (ih->Compression)=0;
    (ih->imageSize)=_img_size;
    (ih->xPixelsPerM)=_width;
    (ih->yPixelsPerM)=_height;
    (ih->colorsUsed)=255; //p
    (ih->colorsImportant)=0;
    /////////////////////////////////////
    for(int i=0;i<_img_size;i++)
    {
        colourTable[i].red=255;
        colourTable[i].green=0;
        colourTable[i].blue=0;
        colourTable[i].reserved=0;
    }
    SaveFile(buffer,_file_size,"0.bmp");
}


void SaveFile(void* buffer,int bufferSize,char* fileName)
{
    int file=open(fileName,O_WRONLY|O_CREAT,0666);
    write(file,buffer,bufferSize);
    close(file);
}