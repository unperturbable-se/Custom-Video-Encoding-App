/////#include <stdio.h>
/////#include <unistd.h>
/////#include <string.h>
/////#include <fcntl.h>
/////#include <stdint.h>
/////#include <stdlib.h>
/////
/////#define _width 1000
/////#define _height 1000
/////#define _img_size _width*_height
/////#define _file_size 54+4*_img_size
/////#pragma pack(1)
/////
/////typedef struct Pixel
/////{
///// uint8_t blue;
///// uint8_t green;
///// uint8_t red;
///// //uint8_t reserved;
/////} Pixel;
/////
/////
/////typedef struct Header
/////{
/////    uint8_t signature[2];
/////    uint32_t file_size;
/////    uint32_t reserved;
/////    uint32_t data_offset;
/////} Header;
/////
/////typedef struct InfoHeader
/////{
/////  uint32_t size;
/////  uint32_t width;
/////  uint32_t height;
/////  uint16_t planes;
/////  uint16_t bitCount;
/////  uint32_t Compression;
/////  uint32_t imageSize;
/////  uint32_t xPixelsPerM;
/////  uint32_t yPixelsPerM;
/////  uint32_t colorsUsed;
/////  uint32_t colorsImportant;
/////} InfoHeader;
/////
/////void SaveFile(void* buffer,int bufferSize,char* fileName);
/////void makeCircle(Pixel* colourTable,int radius);
/////
/////int main()
/////{
/////    uint8_t* buffer=(void*)malloc(_file_size);
/////    Header* h=(Header*)&buffer[0];
/////    InfoHeader* ih= (InfoHeader*)&buffer[14];
/////    Pixel* colourTable=(Pixel*)&buffer[54];
/////    /////////////////////////////////////
/////    (h->signature)[0]='B';
/////    (h->signature)[1]='M';
/////    (h->file_size)=_file_size;
/////    (h->data_offset)=sizeof(Header)+sizeof(InfoHeader); //placeholder
/////
/////    (ih->size)= sizeof(InfoHeader);
/////    (ih->width)=_width;
/////    (ih->height)=_height;
/////    (ih->planes)=1;
/////    (ih->bitCount)=24; //bits per pixel
/////    (ih->Compression)=0;
/////    (ih->imageSize)=_img_size;
/////    (ih->xPixelsPerM)=_width;
/////    (ih->yPixelsPerM)=_height;
/////    (ih->colorsUsed)=255; //p
/////    (ih->colorsImportant)=0;
/////    /////////////////////////////////////
/////    int radius=0;
/////    char fileName[30];
/////    for(int i=0;i<200;i++)
/////    {
/////      sprintf(fileName,"bmp_images/%d.bmp",i);
/////      makeCircle(colourTable,radius);    
/////      SaveFile(buffer,_file_size,fileName);
/////      radius+=5;
/////      radius%=_width*_height/4;
/////    }
/////}
/////
/////void makeCircle(Pixel* colourTable,int radius)
/////{
/////  for(int y=0;y<_width;y++)
/////    for(int x=0;x<_height;x++)
/////        {
/////            int index=x*_width+y;
/////            int x_transformed=x-0;
/////            int y_transformed=y-0;
/////            int res=x_transformed*x_transformed+y_transformed*y_transformed;
/////            if((res)<radius*radius)
/////            {
/////                colourTable[index].blue=255;
/////                colourTable[index].red=0;
/////                colourTable[index].green=0;
/////            }
/////            else
/////            {
/////               colourTable[index].blue=0;
/////               colourTable[index].red=255;
/////               colourTable[index].green=0; 
/////            }
/////        }
/////}
/////
/////void SaveFile(void* buffer,int bufferSize,char* fileName)
/////{
/////    int file=open(fileName,O_WRONLY|O_CREAT,0666);
/////    write(file,buffer,bufferSize);
/////    close(file);
/////}
//#include "bmp_format.h"
//#include <stdio.h>
//#include <string.h>
//
//int main()
//{
//
//    BMP_Image BIM(1000,1000);
//    int radius=0;
//    char fileName[30];
//    for(int i=0;i<200;i++)
//    {
//      sprintf(fileName,"bmp_images/%d.bmp",i);
//      BIM.makeCircle(radius);    
//      BIM.SaveFile(fileName);
//      radius+=5;
//      radius%=1000*1000/4;
//    }
//}
