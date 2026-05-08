#include <gui_abstraction.h>
#include "bmp_format.h"
#include "avi_format.h"
#include <pthread.h>
#include "globals.h"
#include <stdio.h>

void extractImages();
void createExpandingCircle();
int main() 
{
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
    createExpandingCircle();
    pthread_join(thread,NULL); 
    extractImages();
    return 0;
}


void createExpandingCircle()
{
    BMP_Image BIM(100,100);
    int radius=0;
    char fileName[30];
    for(int i=0;i<g_num_images&& !g_abort_process;i++)
    {
      sprintf(fileName,"bmp_images/%d.bmp",i);
      printf("\n%s being created",fileName);
      BIM.makeCircle(radius);    
      BIM.SaveFile(fileName);
      radius+=1;
      //radius%=50;
      sleep(1);
    }
}

void extractImages()
{
    BMP_Image* arr[g_num_images];
    uint8_t* buffers[g_num_images];
    char fileName[30];
    for(int i=0;i<g_num_images;i++)
    {
        printf("%d",i);
        sprintf(fileName,"bmp_images/%d.bmp",i);
        printf("%d",i);
        //printf("\n%s being created",fileName);
        arr[i]=new BMP_Image(fileName);
        printf("%d",i);
        buffers[i]=arr[i]->getBuffer();
    }
    aviConverter(buffers,g_num_images,100,100,"hello.avi");
    for(int i=0;i<100;i++)delete arr[i];

}
