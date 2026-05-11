#include "gui_abstraction.h"
#include "bmp_format.h"
#include "avi_format.h"
#include <pthread.h>
#include "globals.h"
#include <stdio.h>

void* BmpToAvi(void*);
void* AviToBmp(void*);
void* createExpandingCircle(void*);
void threadScheduler(int processNum,void*fptr(void*));

int main() 
{
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
    pthread_mutex_init(&g_images_decoded_lock,NULL);
    threadScheduler(0,createExpandingCircle);
    //threadScheduler(1,BmpToAvi);
    BmpToAvi(NULL);
    threadScheduler(2,AviToBmp);
    pthread_join(thread,NULL);
    return 0;
}

void threadScheduler(int processNum,void*fptr(void*))
{
 pthread_t threads[99];
 for(int i=0;i<g_num_threads[processNum];i++)pthread_create(&threads[i],NULL,fptr,NULL);
 for(int i=0;i<g_num_threads[processNum];i++)pthread_join(threads[i],NULL);
}



void* createExpandingCircle(void*)
{

    while(!g_start_creating)continue;
    BMP_Image BIM(100,100);
    int radius=0;
    char fileName[30];
    for(int i=++g_images_created;i<g_num_images&&!g_abort_process;i=++g_images_created)
    {
      sprintf(fileName,"bmp_images/%d.bmp",i);
      printf("\n%s being created",fileName);
      BIM.makeCircle(radius);    
      BIM.SaveFile(fileName);
      radius+=1;
    }
    return NULL;
}

void* BmpToAvi(void*)
{
    static BMP_Image** arr=new BMP_Image*[g_num_images];
    static uint8_t** buffers=new uint8_t*[g_num_images];
    static int saved=false;
    while(!g_start_encoding)continue;
    char fileName[30];
    for(int i=++g_images_encoded;i<g_num_images&&!g_abort_process;i=++g_images_encoded)
    {
        //printf("%d",i);
        sprintf(fileName,"bmp_images/%d.bmp",i);
        //printf("%d",i);
        printf("\n%s being encoded",fileName);
        arr[i]=new BMP_Image(fileName);
        //printf("%d",i);
        buffers[i]=arr[i]->buffer;
    }
    if(saved)return NULL;
    saved=true;
    Avi_Video vid(buffers,g_num_images,100,100);
    vid.saveFile("yolo.avi");
    //for(int i=0;i<100;i++)delete arr[i];
    return NULL;
}

void* AviToBmp(void*)
{
    static Avi_Video vid("yolo.avi");
    static int numFrames,width,height;
    static uint8_t** frames=vid.decode(&numFrames,&width,&height);
    while(!g_start_decoding)continue;
    if(g_images_decoded>=numFrames)return NULL;
    BMP_Image img(width,height);
    char fileName[30];
    //printf("\nNum frames:%d  width:%d height: %d",numFrames,width,height);
    pthread_mutex_lock(&g_images_decoded_lock);
    for(int i=g_images_encoded;i<numFrames && !g_abort_process;i=g_images_encoded++)
    {
        pthread_mutex_unlock(&g_images_decoded_lock);
        pthread_mutex_lock(&g_images_decoded_lock);
        //sleep(1);
        sprintf(fileName,"bmp_images/%d.bmp",i);
        printf("\n%s being decoded",fileName);
        memcpy(img.colourTable,frames[i],3*width*height);
        img.SaveFile(fileName);
        //sleep(1);
    }
    pthread_mutex_unlock(&g_images_decoded_lock); 
    return NULL; 
}
