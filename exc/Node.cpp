#include "Node.hpp"
#include "Node/NodePin.hpp"
#include "imgui.h"
#include <Node/NodeFlow.hpp>
#include <iostream>
void Node::Draw()
{
    const NodeFlow::CameraView &cv = GetFlow().GetCameraView();
    const ImVec2 final_pos = cv.Point2Screen({position.x,position.y});
    const ImVec2 final_size = cv.Dir2Screen({size.x,size.y});
    ImGui::SetNextWindowPos(final_pos, ImGuiCond_Always, {0.5, 0.5});
    if (ImGui::BeginChild(ID, cv.Dir2Screen(ImVec2{200, 200}), ImGuiChildFlags_Borders))
    {
    }
    ImGui::EndChild();
    DrawPins(NodePinType::eInput);
    DrawPins(NodePinType::eOutput);
    // ImGui::GetWindowDrawList()->AddRectFilled(final_pos - final_size / 2.0f,
    //                                           final_pos + final_size / 2.0f,
    //                                           ImGui::GetColorU32(ImVec4{1, 1, 1, 1}));
}
void Node::DrawPins(NodePinType t)
{
    const ImVec2 windowCenter = ImVec2{position.x,position.y};
    const ImVec2 windowSize = ImVec2{size.x,size.y};
    const auto& cameraView = GetFlow().GetCameraView();
    const ImVec2 centerEdge = windowCenter + ImVec2{t == NodePinType::eInput ? -windowSize.x : windowSize.x, 0};
    const auto &PinCol = t == NodePinType::eInput ? InputPins : OutputPins;
    ImVec2 totalsize = {0, 0};
    for (const auto &pin : PinCol)
    {
        
        totalsize.x = glm::max(totalsize.x, pin->GetAABB().size().x);
        totalsize.y += pin->GetAABB().size().y;
    }
    ImVec2 startPos = centerEdge - ImVec2{0,totalsize.y/2};
     for (const auto &pin : PinCol) {
        const ImVec2 pinSize = ImVec2{pin->GetAABB().size().x,pin->GetAABB().size().y};
        ImGui::GetWindowDrawList()->AddRect(cameraView.Point2Screen(startPos + pinSize),cameraView.Point2Screen( startPos - pinSize), ImGui::GetColorU32(ImVec4(1.0f,1.0f,1.0f,1.0f)));
        startPos.y += pinSize.y;
    }
}

