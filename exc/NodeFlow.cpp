#include "NodeFlow.hpp"
#include "Node/Node.hpp"
#include "NodeRegistry.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "imgui.h"
void NodeFlow::AddNodeOverlay()
{
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("AddNodeList");
    }
    if (ImGui::BeginPopup("AddNodeList"))
    {
        static char search[128] = "";

        // Optional: auto-focus when popup opens
        if (ImGui::IsWindowAppearing())
            ImGui::SetKeyboardFocusHere();

        ImGui::TextUnformatted("Search");

        ImGui::InputTextWithHint("##search", "Type to search...", search, IM_ARRAYSIZE(search));

        ImGui::Separator();
        std::vector<NodeRegistry::NodeEntry> nodeTypes;

        NodeRegistry::Get().ForEach_NodeType([&](const NodeRegistry::NodeEntry &e)
                                             { nodeTypes.push_back(e); });
        for (const auto &nodeType : nodeTypes)
        {
            if (ImGui::Selectable(nodeType.Name.c_str()))
            {
                std::shared_ptr<Node> newnode = nodeType.Create(*this);
                newnode->SetPosition(cameraView.Point2View(ImGui::GetMousePos()) ); 
                nodes.push_back(newnode);

                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}
void NodeFlow::Draw()
{
    if (ImGui::BeginChild("nodeflow", size, false, ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_NoScrollbar))
    {
        CameraControlsUpdate();
        ComputeView();
        DrawMenuBar();
        DrawView();
        AddNodeOverlay();
    }
    ImGui::EndChild();
}
void NodeFlow::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("test"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void NodeFlow::DrawView()
{
    for (const auto& node : nodes)
    {
        node->Draw();
    }
}
void NodeFlow::ComputeView()
{
    ImVec2 windowPos = ImGui::GetWindowPos();
    cameraView.ScreenToView = glm::mat3(cameraView.CameraZoom, 0, 0, 0, cameraView.CameraZoom, 0,
                                        cameraView.CameraPos.x-windowPos.x, cameraView.CameraPos.y-windowPos.y, 1.0f);
    cameraView.ViewToScreen = glm::inverse(cameraView.ScreenToView);
}
void NodeFlow::CameraControlsUpdate() {
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle) && ImGui::IsWindowHovered())
    {
        cameraView.CameraPos -= ImGui::GetIO().MouseDelta;
    }
}
