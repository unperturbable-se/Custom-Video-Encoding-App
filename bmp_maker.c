#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define _width 20
#define _height 20
#define _img_size _width*_height
#define _file_size 54+4*_img_size

typedef struct Pixel
{
 unsigned char red;
 unsigned char green;
 unsigned char blue;
 unsigned char reserved;
} Pixel;


typedef struct Header
{
    char signature[2];
    int file_size;
    int reserved;
    int data_offset;
} Header;

typedef struct InfoHeader
{
  int size;
  int width;
  int height;
  short planes;
  short bitCount;
  int Compression;
  int imageSize;
  int xPixelsPerM;
  int yPixelsPerM;
  int colorsUsed;
  int colorsImportant;
} InfoHeader;


int main()
{
    char* buffer=(void*)malloc(_file_size);
    Header* h=(Header*)&buffer[0];
    InfoHeader* ih= (InfoHeader*)&buffer[14];
    Pixel* colourTable=(Pixel*)&buffer[54];
    /////////////////////////////////////
    (h->signature)[0]='B';
    (h->signature)[1]='M';
    (h->file_size)=_file_size;
    (h->data_offset)=0; //placeholder

    (ih->size)= _img_size;
    (ih->width)=_width;
    (ih->height)=_height;
    (ih->planes)=1;
    (ih->bitCount)=24; //bits per pixel
    (ih->Compression)=0;
    (ih->imageSize)=0;
    (ih->xPixelsPerM)=_width;
    (ih->yPixelsPerM)=_height;
    (ih->colorsUsed)=0; //placeholder
    (ih->colorsImportant)=0;
    /////////////////////////////////////
    SaveFile(buffer,_file_size,"0.bmp");
}





int SaveFile(void* buffer,int bufferSize,char* fileName)
{
    int file=open(fileName,O_WRONLY|O_CREAT,0666);
    write(file,buffer,bufferSize);
    close(file);
    return 0;
}