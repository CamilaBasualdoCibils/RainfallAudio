
#include "RainfallEditor.hpp"

#include "GUI/GUI.hpp"
#include "imgui.h"
#include <iostream>
#include <stdexcept>

RainfallEditor::RainfallEditor() : AppWindow(1920, 1080, "Rainfall Editor") {}
void RainfallEditor::Run()
{
    if (!Init())
        throw std::runtime_error("");
    using Clock = std::chrono::high_resolution_clock;

    auto lastTime = Clock::now();
    while (!ShouldClose())
    {
        auto currentTime = Clock::now();
        std::chrono::duration<double> delta = currentTime - lastTime;
        DeltaTime = delta.count(); // seconds
        lastTime = currentTime;

        PollEvents();
        BeginFrame();
        ImGui::ShowDemoWindow();
        GUI::Get().Draw();

        EndFrame();
    }
}
