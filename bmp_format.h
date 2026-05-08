#pragma once
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
//#include <semaphore>

extern int g_saves;
extern bool g_abort_process;
extern int g_num_threads;
//extern sem_t g_threader;

#pragma pack(1)

typedef struct Pixel
{
 uint8_t blue;
 uint8_t green;
 uint8_t red;
 //uint8_t reserved;
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


class BMP_Image
{
    int width,height,file_size,img_size;
    Pixel* colourTable;
    uint8_t* buffer;
    public:
    BMP_Image(int width,int height):width{width},height{height}
    {
        img_size=width*height;
        file_size=54+4*img_size;
        buffer=(uint8_t*)malloc(file_size);
        Header* h=(Header*)&buffer[0];
        InfoHeader* ih= (InfoHeader*)&buffer[14];
        colourTable=(Pixel*)&buffer[54];
        (h->signature)[0]='B';
        (h->signature)[1]='M';
        (h->file_size)=file_size;
        (h->data_offset)=sizeof(Header)+sizeof(InfoHeader); //placeholder

        (ih->size)= sizeof(InfoHeader);
        (ih->width)=width;
        (ih->height)=height;
        (ih->planes)=1;
        (ih->bitCount)=24; //bits per pixel
        (ih->Compression)=0;
        (ih->imageSize)=img_size;
        (ih->xPixelsPerM)=width;
        (ih->yPixelsPerM)=height;
        (ih->colorsUsed)=255; //p
        (ih->colorsImportant)=0;
    }

    void SaveFile(char* fileName)
    {
    int file=open(fileName,O_WRONLY|O_CREAT,0666);
    write(file,buffer,file_size);
    close(file);
    g_saves++;
    }

    void makeCircle(int radius)
    {
     for(int y=0;y<width;y++)
       for(int x=0;x<height;x++)
           {
               int index=x*width+y;
               int x_transformed=x-0;
               int y_transformed=y-0;
               int res=x_transformed*x_transformed+y_transformed*y_transformed;
               if((res)<radius*radius)
               {
                   colourTable[index].blue=255;
                   colourTable[index].red=0;
                   colourTable[index].green=0;
               }
               else
               {
                  colourTable[index].blue=0;
                  colourTable[index].red=255;
                  colourTable[index].green=0; 
               }
           }
    }
};

