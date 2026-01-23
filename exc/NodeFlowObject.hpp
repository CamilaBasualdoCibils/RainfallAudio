#pragma once

#include "glm/fwd.hpp"
#include "imgui.h"
#include <memory>
#include <vector>
#include <geometry/AABB.hpp>
class NodeFlow;
class NodeFlowObject
{
    
    const NodeFlow& nf;
    NodeFlowObject* parent;
    std::vector<std::shared_ptr<NodeFlowObject>> ChildrenObjects;
    public:
    NodeFlowObject(const NodeFlow& nf):nf(nf),parent(nullptr){}
    virtual ~NodeFlowObject() = default;
    const NodeFlow& GetFlow() const {return nf;}
    virtual AABB2f GetAABB() const;
    void SetParent(NodeFlowObject* par){parent = par;}
    protected:
    glm::vec2 position,size;

};