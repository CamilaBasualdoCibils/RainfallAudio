#include "Port.hpp"
#include "NodeFlow/Node.hpp"
#include <QPainter>
#include <QWidget>
#include <qnamespace.h>
#include <qpoint.h>
#include <qvectornd.h>
#include "NodeFlow.hpp"
IPort::IPort(NodeFlow *flow, NodeID parentID,PortID ID) : QGraphicsItem(flow->GetNode(parentID).get()), flow(flow),ParentID(parentID),ID(ID) {
    setAcceptedMouseButtons(Qt::LeftButton);
}
void IPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Background
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::blue, 2));
    painter->drawRoundedRect(boundingRect(), 1, 1);
  
}
QRectF IPort::boundingRect() const
{
    QPointF size = {10,10};
    return QRectF(-size/2.0f, size); // width 120, height 60
}
