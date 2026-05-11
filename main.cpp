#include "gui_abstraction.h"
#include "bmp_format.h"
#include "avi_format.h"
#include <pthread.h>
#include "globals.h"
#include <stdio.h>
#include "unistd.h"

void* BmpToAvi(void*);
void* AviToBmp(void*);
void* createExpandingCircle(void*);
void threadScheduler(int processNum,void*fptr(void*));

int main() 
{
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
    pthread_mutex_init(&g_images_decoded_lock,NULL);
    pthread_mutex_init(&g_images_created_lock,NULL);
    pthread_mutex_init(&g_images_encoded_lock,NULL);
    threadScheduler(0,createExpandingCircle);
    threadScheduler(1,BmpToAvi);
    //BmpToAvi(NULL);
    threadScheduler(2,AviToBmp);
    //AviToBmp(NULL);
    pthread_join(thread,NULL);
    return 0;
}

void threadScheduler(int processNum,void*fptr(void*))
{
 pthread_t threads[100];
 for(int i=0;i<g_num_threads[processNum];i++)pthread_create(&threads[i],NULL,fptr,NULL);
 for(int i=0;i<g_num_threads[processNum];i++)pthread_join(threads[i],NULL);
}



void* createExpandingCircle(void*)
{
    static BMP_Image BIM(100,100);
    while(!g_start_creating)usleep(100000);
    int radius=0;
    char fileName[30];
    pthread_mutex_lock(&g_images_created_lock);
    for(int i=g_saves;i<g_num_images&&!g_abort_process;i=g_saves)
    {
      //pthread_mutex_unlock(&g_images_created_lock);
      sprintf(fileName,"bmp_images/%d.bmp",i);
      printf("\n%s being created",fileName);
      radius=i*2;
      BIM.makeCircle(radius);    
      BIM.SaveFile(fileName);
      pthread_mutex_unlock(&g_images_created_lock);
      usleep(100000);
      pthread_mutex_lock(&g_images_created_lock);
    }
    pthread_mutex_unlock(&g_images_created_lock);
    return NULL;
}

void* BmpToAvi(void*)
{
    static BMP_Image** arr=new BMP_Image*[g_num_images];
    static uint8_t** buffers=new uint8_t*[g_num_images];
    static int saved=false;
    while(!g_start_encoding)usleep(100000);
    char fileName[30];
    pthread_mutex_lock(&g_images_encoded_lock);
    for(int i=g_images_encoded++;i<g_num_images&&!g_abort_process;i=g_images_encoded++)
    {
        //printf("%d",i);
        sprintf(fileName,"bmp_images/%d.bmp",i);
        //printf("%d",i);
        printf("\n%s being encoded",fileName);
        arr[i]=new BMP_Image(fileName);
        //printf("%d",i);
        buffers[i]=arr[i]->buffer;
        pthread_mutex_unlock(&g_images_encoded_lock);
        usleep(100000);
        pthread_mutex_lock(&g_images_encoded_lock);
    }
    pthread_mutex_unlock(&g_images_encoded_lock);
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
    static int numFrames=100,width,height;
    static uint8_t** frames=vid.decode(&numFrames,&width,&height);
    //numFrames=100,width=100,height=100;
    while(!g_start_decoding)usleep(100000);
    if(g_images_decoded>=numFrames)return NULL;
    BMP_Image img(width,height);
    char fileName[30];
    //printf("\nNum frames:%d  width:%d height: %d",numFrames,width,height);
    pthread_mutex_lock(&g_images_decoded_lock);
    for(int i=g_images_decoded++;i<numFrames && !g_abort_process;i=g_images_decoded++)
    {
        sprintf(fileName,"bmp_images/%d.bmp",i);
        printf("\n%s being decoded",fileName);
        memcpy(img.colourTable,frames[i],3*width*height);
        img.SaveFile(fileName);
        g_images_decoded++;
        pthread_mutex_unlock(&g_images_decoded_lock);
        usleep(100000);
        pthread_mutex_lock(&g_images_decoded_lock);
    }
    pthread_mutex_unlock(&g_images_decoded_lock); 
    return NULL; 
}
