#include <gui_abstraction.h>
#include "imgui/imgui.h"
#include <pthread.h>

void Window::windowHandle()
{
  static int counter = 0;
  static double progress=0;
  ImGui::Text("Avi to bmp video encoder, yayyy!");
  if (ImGui::Button("Button")) counter++;
  ImGui::Text("Number of images processed: %d", counter);
  ImGui::ProgressBar(progress);
  progress+=0.01;
  counter+=50;
  if(progress>=1)g_closeWindow=true;
  ImGui::End();  
}

int main() 
{
    pthread_t thread;
    pthread_create(&thread,NULL,guiMain,NULL);
    pthread_join(thread,NULL);
    return 0;
}
