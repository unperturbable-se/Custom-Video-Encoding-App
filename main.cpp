#include "gui_abstraction.h"
#include "bmp_format.h"
#include "avi_format.h"
#include <pthread.h>
#include "globals.h"
#include <stdio.h>

void* BmpToAvi(void*);
void* AviToBmp(void*);
void createExpandingCircle();
int main() 
{
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
    createExpandingCircle();
    pthread_join(thread,NULL); 
    //BmpToAvi();
    pthread_mutex_init(&g_images_processed_lock,NULL);
    pthread_t threads[100];
    for(int i=0;i<100;i++)pthread_create(&threads[i],NULL,AviToBmp,NULL);
    for(int i=0;i<100;i++)pthread_join(threads[i],NULL);
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
    }
}

void* BmpToAvi(void*)
{
    BMP_Image* arr[g_num_images];
    uint8_t* buffers[g_num_images];
    char fileName[30];
    for(int i=0;i<g_num_images;i++)
    {
        //printf("%d",i);
        sprintf(fileName,"bmp_images/%d.bmp",i);
        //printf("%d",i);
        //printf("\n%s being created",fileName);
        arr[i]=new BMP_Image(fileName);
        //printf("%d",i);
        buffers[i]=arr[i]->buffer;
    }
    Avi_Video vid(buffers,g_num_images,100,100);
    vid.saveFile("yolo.avi");
    for(int i=0;i<100;i++)delete arr[i];
}

void* AviToBmp(void*)
{
    static Avi_Video vid("hello.avi");
    static int numFrames,width,height;
    static uint8_t** frames=vid.decode(&numFrames,&width,&height);
    if(g_images_processed>=numFrames)return NULL;
    BMP_Image img(width,height);
    char fileName[30];
    //printf("\nNum frames:%d  width:%d height: %d",numFrames,width,height);
    pthread_mutex_lock(&g_images_processed_lock);
    for(int i=g_images_processed;i<numFrames && !g_abort_process;i=g_images_processed)
    {
        pthread_mutex_unlock(&g_images_processed_lock);
        //sleep(1);
        sprintf(fileName,"bmp_images/%d.bmp",i);
        memcpy(img.colourTable,frames[i],3*width*height);
        img.SaveFile(fileName);
        //sleep(1);
        pthread_mutex_unlock(&g_images_processed_lock);
    } 
    return NULL; 
}
