#pragma once
#include "imgui/imgui.h"
#include "implot/implot.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

extern bool g_closeWindow;

class Window
{
    GLFWwindow* window;
    public:
    Window()
    {
      glfwInit();
      window=glfwCreateWindow(1280,720,"AVI/BMP Video Encoder",NULL,NULL);
      glfwMakeContextCurrent(window);
      glfwSwapInterval(1);
      ImGui::CreateContext();
      ImPlot::CreateContext();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init("#version 130");

    }
    ~Window()
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
      ImPlot::DestroyContext();
      glfwDestroyWindow(window);
      glfwTerminate();
    }
     
    void beginPolling()
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void endPolling()
    {
        ImGui::Render();
        int ww, wh;
        glfwGetFramebufferSize(window, &ww, &wh);
        glViewport(0, 0, ww, wh);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    void windowHandle();
    void poll()
    {
        beginPolling();
        windowHandle();
        endPolling();
    }
    operator GLFWwindow*()
    {
      return window;
    }
};


void* guiMain(void*);