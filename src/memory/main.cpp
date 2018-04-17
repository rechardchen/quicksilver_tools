#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <thread>
#include "QSMemoryProfiler.h"
#include "QSSmallPoolProfiler.h"


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}


int main()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "QSGame Remote Profiler", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gl3wInit();

    //Setup Imgui binding
    ImGui_ImplGlfwGL3_Init(window, true);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //profilers
    //TODO: use thread pool
    QSMemoryProfiler MemProfiler("memprofile");
    QSMemoryView MemView(MemProfiler);
    std::thread ThreadMem([&]{
        MemProfiler.Serve();
    });

    QSSmallPoolProfiler SmallPoolProfiler("chunkprof");
    QSSmallPoolView SmallPoolView(SmallPoolProfiler);
    std::thread ThreadSmallPool([&] {
        SmallPoolProfiler.Serve();
    });

    while(!glfwWindowShouldClose(window))
    {
        MemView.CheckStats(glfwGetTime());
        glfwPollEvents();

        ImGui_ImplGlfwGL3_NewFrame();

        {
            ImGui::Text("QSGame Remote Profilers");
            ImGui::ColorEdit3("Background Color", (float*)&clear_color);
            ImGui::Spacing();
            if (ImGui::Button("Memory Profiler"))
            {
                MemView.ToggleVisible();
            }
            if (ImGui::Button("Streaming Profiler")) {}

            if (ImGui::Button("Small Pool Profiler"))
            {
                SmallPoolView.ToggleVisible();
            }
        }

        MemView.ImDraw(glfwGetTime());
        SmallPoolView.ImDraw();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
    //QSMemoryProfiler profiler("memprofile");
    //profiler.Serve();
}