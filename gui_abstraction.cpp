#include <gui_abstraction.h>
#include "bmp_format.h"
#include <time.h>


void ImageMaker_windowHandle();
void ImageEncoder_windowHandle();
void ImageDecoder_windowHandle();
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
      if(!g_start_creating)
      {
        ImGui::Begin("Image Creation");
        ImageMaker_windowHandle();
        ImGui::End();
      }

      if(!g_start_encoding)
      {
        ImGui::Begin("Image Encoding");
        ImageEncoder_windowHandle();
        ImGui::End();
      }

      if(!g_start_decoding)
      {
        ImGui::Begin("Image Decoding");
        ImageDecoder_windowHandle();
        ImGui::End();
      }
      
      ImGui::Begin("Toggle Menu");
      userToggleMenu_windowHandle();
      ImGui::End();
}

void ImageMaker_windowHandle()
{
  static time_t start=time(NULL);
  int elapsed=static_cast<int>(difftime(time(NULL),start));
  float progress=(float)g_images_created/g_num_images;
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
  float progress=(float)g_images_encoded/g_num_images;
  static int images_saved[100];
  images_saved[elapsed]=g_images_encoded/2;
  ImGui::Text("bmp to avi video encoding");
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

void ImageDecoder_windowHandle()
{
  static time_t start=time(NULL);
  int elapsed=static_cast<int>(difftime(time(NULL),start));
  float progress=(float)g_images_decoded/g_num_images;
  static int images_saved[100];
  images_saved[elapsed]=g_images_decoded;
  ImGui::Text("Avi to bmp video decoding");
  if (ImGui::Button("Abort Process")){ g_abort_process=true;g_closeWindow=true;}


  ImGui::Text("decoding progress:");          
  ImGui::ProgressBar(progress);
  ImGui::SameLine();
  ImGui::Text("%.1f%%", progress * 50.0f);
  ImGui::SameLine();
  ImGui::Text("Progress:%.2f\%",progress*50);

  ImPlot::SetNextAxesLimits(0,100,0,50);
  if(ImPlot::BeginPlot("image decoding progress"))
    {
      //ImPlot::PlotHistogram2D("Images encoded,")
      ImPlot::PlotLine("num images decoded",images_saved,elapsed);
      ImPlot::EndPlot();
    }

  //if(g_saves>=99 && progress>=1)g_closeWindow=true;
}


void userToggleMenu_windowHandle()
{
    ImGui::Text("Number of threads allocated");
    ImGui::SliderInt("Worker 1",&g_num_threads[0],2,100);
    ImGui::SliderInt("Worker 2",&g_num_threads[1],2,100);
    ImGui::SliderInt("Worker 3",&g_num_threads[2],2,100);  

    g_start_creating=ImGui::Button("Start Image Creation");
    g_start_encoding=ImGui::Button("Start Image Encoding");
    g_start_decoding=ImGui::Button("Start Video Decoding");
}