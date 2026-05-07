#include <gui_abstraction.h>
#include "bmp_format.h"
#include "imgui/imgui.h"
#include <pthread.h>
bool g_closeWindow=false;
int g_saves=0;
bool g_abort_process=false;

void Window::windowHandle()
{
  static int counter = 0;
  float progress=(float)g_saves/50;
  ImGui::Text("Avi to bmp video encoder");
  if (ImGui::Button("Abort Process")) g_abort_process=true;
  ImGui::Text("Encoding progress:");          
  ImGui::ProgressBar(progress);
  ImGui::SameLine();
  ImGui::Text("%.1f%%", progress * 100.0f);
  ImGui::SameLine();
  ImGui::Text("Progress:%.2f\%",progress*100);
  if(g_saves>=99 && progress>=1)g_closeWindow=true;
  ImGui::End();  
}

int main() 
{
    g_saves=0;
    g_closeWindow=false;
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
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
      radius%=50;
      sleep(1);
    }
    //pthread_join(thread,NULL);
    return 0;
}

