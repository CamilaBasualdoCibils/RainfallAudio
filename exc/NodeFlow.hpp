#pragma once
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>

#include "Node.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "imgui.h"
class NodeFlowView
{
    ImVec2 size = {0, 0};

  public:
    void Draw();
    float CameraZoom_Target = 1;
    ImVec2 CameraPos_Target = {0, 0};
    struct CameraView
    {
        float CameraZoom = 1;
        ImVec2 CameraPos = {0, 0};
        glm::mat3 ViewToScreen, ScreenToView;
        ImVec2 Point2View(ImVec2 p) const
        {
            glm::vec2 pr = ScreenToView * glm::vec3(p.x, p.y, 1.0);
            return {pr.x, pr.y};
        };
        ImVec2 Point2Screen(ImVec2 p) const
        {
            glm::vec2 pr = ViewToScreen * glm::vec3(p.x, p.y, 1.0);
            return {pr.x, pr.y};
        }
        ImVec2 Dir2View(ImVec2 d) const
        {
            glm::vec2 pr = ScreenToView * glm::vec3(d.x, d.y, 0.0);
            return {pr.x, pr.y};
        }
        ImVec2 Dir2Screen(ImVec2 d) const
        {
            glm::vec2 pr = ViewToScreen * glm::vec3(d.x, d.y, 0.0);
            return {pr.x, pr.y};
        }
    } cameraView;
    const CameraView &GetCameraView() const { return cameraView; }

  private:
    void DrawMenuBar();
    void DrawView();
    void AddNodeOverlay();
    void ComputeView();
    void CameraControlsUpdate();
    boost::multi_index_container<
        std::shared_ptr<Node>,
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<>
            >>
        nodes;
};