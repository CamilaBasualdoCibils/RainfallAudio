#include "Connection.hpp"
#include "NodeFlow.hpp"

QPointF Connection::GetPortScenePos(PortRef p) const { 
    return flow->GetPort(p)->scenePos();
}
