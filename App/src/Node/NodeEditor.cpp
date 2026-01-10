#include "NodeEditor.hpp"
#include "Node/NodeRegistry.hpp"
#include "RainfallEditor.hpp"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
bool NodeEditor::Draw(const std::string_view label, ImVec2 size)
{
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext &g = *ImGui::GetCurrentContext();
    const ImGuiStyle &style = g.Style;
    const auto avail_Space = ImGui::GetContentRegionAvail();
    ImVec2 finalSize = size;
    finalSize.x = (finalSize.x == 0) ? avail_Space.x : finalSize.x;
    finalSize.y = (finalSize.y == 0) ? avail_Space.y : finalSize.y;
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, pos + finalSize);
    ImGuiID id = window->GetID(label.data());
    // ImGui::SetNextItemAllowOverlap();
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;
    ImGui::PushClipRect(bb.Min, bb.Max, true);

    ImDrawList *draw = window->DrawList;

    draw->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(BackgroundColor), 0);

    auto translate = [](float tx, float ty)
    { return glm::mat3{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, tx, ty, 1.0f}; };

    auto scale = [](float sx, float sy)
    { return glm::mat3{sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f}; };

    const float screenHalfX = bb.GetSize().x * 0.5f;
    const float screenHalfY = bb.GetSize().y * 0.5f;

    const float aspectRatio = bb.GetSize().x / bb.GetSize().y;
    // build transforms (rightmost applied first)
    const glm::mat3 T_negCam = translate(-CameraLocation.x, -CameraLocation.y);
    const glm::mat3 S_cam = scale(screenHalfX / CameraScaleX,
                                  screenHalfY / CameraScaleX * aspectRatio);
    const glm::mat3 T_center = translate(screenHalfX, screenHalfY);
    const glm::mat3 T_bbmin = translate(bb.Min.x, bb.Min.y);

    // full chain
    World2Screen = T_bbmin * T_center * S_cam * T_negCam;
    Screen2World = glm::inverse(World2Screen);

    glm::vec2 WorldMin = Screen2World * glm::vec3(bb.Min.x, bb.Min.y, 1);
    glm::vec2 WorldMax = Screen2World * glm::vec3(bb.Max.x, bb.Max.y, 1);

    // Find the first line positions
    float targetStep =
        CameraScaleX / 5.0f; // heuristic: ~5 lines across half-view

    float exponent = std::floor(glm::log2(targetStep));
    float step = glm::pow(2.0f, exponent);

    glm::ivec2 Start = glm::floor(WorldMin / step);
    glm::ivec2 End = glm::ceil(WorldMax / step);
    // Draw vertical lines
    for (int i = Start.x; i <= End.x; i++)
    {
        float x = (World2Screen * glm::vec3(i * step, 0, 1.0f)).x;
        // float x = WorldToScreenX(i * step);
        draw->AddLine(ImVec2(x, bb.Min.y), ImVec2(x, bb.Max.y),
                      ImGui::GetColorU32(LineColor));
    }

    // Draw horizontal lines
    for (int i = Start.y; i <= End.y; i++)
    {

        float y = (World2Screen * glm::vec3(0, i * step, 1.0f)).y;
        draw->AddLine(ImVec2(bb.Min.x, y), ImVec2(bb.Max.x, y),
                      ImGui::GetColorU32(LineColor));
    }

    Controls();
    DrawNodes();
    ImGui::PopClipRect();

    return true;
    // ImGui::ItemSize()
}

void NodeEditor::Controls()
{
    ImGuiIO &io = ImGui::GetIO();
    CameraScaleX_target += -io.MouseWheel * (CameraScaleX * 0.3f);
    CameraScaleX_target =
        glm::clamp(CameraScaleX_target, CameraScaleRange.x, CameraScaleRange.y);

    CameraScaleX = std::lerp(CameraScaleX, CameraScaleX_target,
                             RainfallEditor::Get().GetDeltaTime() * 20.0f);

    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
    {
        // Mouse delta in screen space (pixels)
        ImVec2 delta = io.MouseDelta;

        // Convert pixels → world units
        // CameraScaleX = how many world units fit in the visible X range
        // We assume uniform scale on X/Y
        float worldPerPixelX =
            CameraScaleX / ImGui::GetContentRegionAvail().x * 2;
        float worldPerPixelY =
            CameraScaleX / ImGui::GetContentRegionAvail().x * 2;

        CameraLocation.x -= delta.x * worldPerPixelX;
        CameraLocation.y -= delta.y * worldPerPixelY; // Y usually inverted
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) &&
        ImGui::IsItemHovered())
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

        ImGui::InputTextWithHint("##search", "Type to search...", search,
                                 IM_ARRAYSIZE(search));

        ImGui::Separator();
        std::vector<NodeRegistry::NodeEntry> nodeTypes;

        NodeRegistry::Get().ForEach_NodeType(
            [&](const NodeRegistry::NodeEntry &e) { nodeTypes.push_back(e); });
        for (const auto &nodeType : nodeTypes)
        {
            if (ImGui::Selectable(nodeType.Name.c_str()))
            {
                // Handle selection here
                // e.g. DoAction(item);
                std::shared_ptr<Node> newnode = nodeType.Create();
                const ImVec2 mouse_pos = ImGui::GetMousePos();
                const glm::vec2 mouse_pos_g = {mouse_pos.x,mouse_pos.y};
                const glm::vec2 Node_pos = Screen2World*glm::vec3(mouse_pos_g,1);
                newnode->SetPosition(Node_pos);
                AddNode(newnode);

                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}
void NodeEditor::AddNode(std::shared_ptr<Node> node) { nodes.push_back(node); }
void NodeEditor::DrawNodes()
{
    int i = 1;
    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        nodes.modify(
            it, [&](std::shared_ptr<Node> node)
            { node->DrawNode(ImGui::GetWindowDrawList(), World2Screen); });
    }
}
