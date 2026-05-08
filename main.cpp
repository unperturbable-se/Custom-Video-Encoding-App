#include <gui_abstraction.h>
#include "bmp_format.h"
#include <pthread.h>
#include "globals.h"

void createExpandingCircle();
int main() 
{
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
    createExpandingCircle();
    pthread_join(thread,NULL); 
    return 0;
}


void createExpandingCircle()
{
    BMP_Image BIM(100,100);
    int radius=0;
    char fileName[30];
    for(int i=0;i<50&& !g_abort_process;i++)
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

