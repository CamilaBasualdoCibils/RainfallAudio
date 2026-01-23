#pragma once

#include "GL.hpp"
#include "GLFW/glfw3.h"
#include "imgui.h"

#include <iostream>
#include <stdexcept>

class AppWindow
{
  public:
    AppWindow(int width, int height, const char *title)
        : m_width(width), m_height(height), m_title(title)
    {
    }

    virtual ~AppWindow() { Shutdown(); }

    // Explicit startup
    bool Init()
    {
        if (!InitGLFW())
            return false;

        if (!InitOpenGL())
            return false;

        if (!InitImGui())
            return false;

        OnInit();
        return true;
    }

    bool ShouldClose() const { return m_window && glfwWindowShouldClose(m_window); }

    void PollEvents() const { glfwPollEvents(); }

    void BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame()
    {
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(m_window);
            // TODO for OpenGL: restore current GL context.
        }
    }
    GLFWwindow *GetWindow() const { return m_window; }

  protected:
    // ---- Virtual extension points ----
    virtual void OnInit() {}
    virtual void OnShutdown() {}

  private:
    bool InitGLFW()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW\n";
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_window);
        std::cout << "Using: " << glGetString(GL_RENDERER) << std::endl;
        glfwSwapInterval(1);
        return true;
    }

    bool InitOpenGL()
    {
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW\n";
            return false;
        }
        return true;
    }

    bool InitImGui()
    {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGui::GetIO().ConfigFlags =
            ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();

        if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true))
            return false;

        if (!ImGui_ImplOpenGL3_Init("#version 330"))
            return false;

        glm::vec2 content_scale;
        glfwGetWindowContentScale(m_window, &content_scale.x, &content_scale.y);
        ImGuiStyle &style = ImGui::GetStyle();
        float scale = content_scale.x;

        style.WindowPadding = style.WindowPadding * scale;
        style.FramePadding = style.FramePadding * scale;
        style.ItemSpacing = style.ItemSpacing * scale;
        style.ItemInnerSpacing = style.ItemInnerSpacing * scale;
        style.IndentSpacing = style.IndentSpacing * scale;
        style.ScrollbarSize = style.ScrollbarSize * scale;
        style.GrabMinSize = style.GrabMinSize * scale;
        ImFontConfig fc;
        fc.SizePixels = 16.0f * scale;
        ImGui::GetIO().Fonts->AddFontDefault(&fc);
        return true;
    }

    void Shutdown()
    {
        OnShutdown();

        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }

        glfwTerminate();
    }

  private:
    GLFWwindow *m_window = nullptr;
    int m_width = 0;
    int m_height = 0;
    const char *m_title = nullptr;
};
