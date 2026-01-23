
#include "GUI.hpp"
#include "GL.hpp"
#include "RainfallEditor.hpp"
#include "imgui.h"
/*
void GUI::Draw()
{
    const auto window = RainfallEditor::Get().GetWindow();
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);

    // Set up full-screen window for dockspace
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize(ImVec2((float)fbw, (float)fbh));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("##DockspaceRoot", nullptr, window_flags);

   //ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    //ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    node_flow.Draw();
    ImGui::End();
    DrawContent();
    ImGui::PopStyleVar(2);
}
*/
void GUI::DrawContent()
{

        
    

}
