#include "Node.hpp" 
#include "glm/fwd.hpp"
#include "imgui.h"
void Node::DrawNode(ImDrawList* drawlist,glm::mat3 World2Screen){

    const auto Size = GetSize();
    
    glm::vec2 Min = Position-Size/2.0f,Max = Position+Size/2.0f;
    Min = World2Screen*(glm::vec3(Min,1));
    Max = World2Screen*(glm::vec3(Max,1));
    float HeaderHeightWorld = (World2Screen*(glm::vec3(0,HeaderHeight,0.0f))).y;
    ImVec4 color = ImVec4{0.5,0.5,0.5,1.0},hovered = ImVec4{0.7,0.7,0.7,1.0};
    drawlist->AddRectFilled(ImVec2{Min.x,Min.y}, ImVec2{Max.x,Max.y}, ImGui::GetColorU32(ImVec4{0,0,0,1}),5);
    drawlist->AddRect(ImVec2{Min.x,Min.y}, ImVec2{Max.x,Max.y}, ImGui::GetColorU32(IsHovered()?hovered:color),5);
    drawlist->AddRectFilled({Min.x,Min.y},{Max.x,Min.y+HeaderHeightWorld},ImGui::GetColorU32({0.7,0.2,0.2,1}),5,ImDrawFlags_RoundCornersTop);
    drawlist->AddText({Min.x,Min.y},ImGui::GetColorU32({1,1,1,1}),GetTitle().c_str());
    DrawPreview();
}
bool Node::IsHovered() const 
{
    //ImGui::GetMousePos()
    return true;
}
