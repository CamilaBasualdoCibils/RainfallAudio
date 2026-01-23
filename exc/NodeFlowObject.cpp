#include "NodeFlowObject.hpp" 
#include "geometry/AABB.hpp"
AABB2f NodeFlowObject::GetAABB() const {
    return AABB2f::FromCenterExtents(position, size/2.0f);
}
