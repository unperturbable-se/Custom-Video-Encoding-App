#include <gui_abstraction.h>
#include "bmp_format.h"
#include <time.h>


void ImageMaker_windowHandle();
void ImageEncoder_windowHandle();
void userToggleMenu_windowHandle();

void* guiMain(void*)
{
  Window window;
  while (!glfwWindowShouldClose(window)) 
  {
      window.poll();
      if(g_closeWindow)glfwSetWindowShouldClose(window, GLFW_TRUE);  
  }
  g_abort_process=true;
  return NULL;
}

void Window::windowHandle()
{
      ImGui::Begin("Image Creation");
      ImageMaker_windowHandle();
      ImGui::End();
      ImGui::Begin("Image Encoding");
      ImageEncoder_windowHandle();
      ImGui::End();
      ImGui::Begin("Toggle Menu");
      userToggleMenu_windowHandle();
      ImGui::End();
}

void ImageMaker_windowHandle()
{
  static time_t start=time(NULL);
  int elapsed=static_cast<int>(difftime(time(NULL),start));
  float progress=(float)g_saves/g_num_images;
  static int images_saved[100];
  images_saved[elapsed]=g_saves;
  ImGui::Text("BMP Image Creation");
  if (ImGui::Button("Abort Process")) {g_abort_process=true;g_closeWindow=true;}


  ImGui::Text("Encoding progress:");          
  ImGui::ProgressBar(progress);
  ImGui::SameLine();
  ImGui::Text("%.1f%%", progress * 100.0f);
  ImGui::SameLine();
  ImGui::Text("Progress:%.2f\%",progress*100);

  ImPlot::SetNextAxesLimits(0,100,0,50);
  if(ImPlot::BeginPlot("image creation progress"))
    {
      ImPlot::PlotLine("num images created",images_saved,elapsed);
      ImPlot::EndPlot();
    }

  //if(g_saves>=99 && progress>=1)g_closeWindow=true;  
}



void ImageEncoder_windowHandle()
{
  static time_t start=time(NULL);
  int elapsed=static_cast<int>(difftime(time(NULL),start));
  float progress=(float)g_images_processed/g_num_images;
  static int images_saved[100];
  images_saved[elapsed]=g_saves/2;
  ImGui::Text("Avi to bmp video encoding");
  if (ImGui::Button("Abort Process")){ g_abort_process=true;g_closeWindow=true;}


  ImGui::Text("Encoding progress:");          
  ImGui::ProgressBar(progress);
  ImGui::SameLine();
  ImGui::Text("%.1f%%", progress * 50.0f);
  ImGui::SameLine();
  ImGui::Text("Progress:%.2f\%",progress*50);

  ImPlot::SetNextAxesLimits(0,100,0,50);
  if(ImPlot::BeginPlot("image encoding progress"))
    {
      //ImPlot::PlotHistogram2D("Images encoded,")
      ImPlot::PlotLine("num images encoded",images_saved,elapsed);
      ImPlot::EndPlot();
    }

  //if(g_saves>=99 && progress>=1)g_closeWindow=true;
}


void userToggleMenu_windowHandle()
{
    ImGui::Text("Number of threads allocated");
    ImGui::SliderInt("Worker 1",&g_num_threads_w1,1,100);
    ImGui::SliderInt("Worker 2",&g_num_threads_w2,1,100);
    ImGui::SliderInt("Worker 3",&g_num_threads_w3,1,100);
    ImGui::SliderInt("Worker 4",&g_num_threads_w4,1,100);
    ImGui::SliderInt("Worker 5",&g_num_threads_w5,1,100);
    
}