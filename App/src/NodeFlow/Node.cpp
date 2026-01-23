#include "Node.hpp"
#include <qnamespace.h>
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Background
    painter->setBrush(isSelected() ? Qt::darkGray : Qt::black);
    painter->setPen(QPen(Qt::darkGray, 2));
    painter->drawRect(boundingRect());

    // Title text
    painter->setPen(Qt::white);
    painter->drawText(boundingRect().adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignTop, m_title);
}
QRectF Node::boundingRect() const
{
    return QRectF(-60, -250.0f/2, 120, 250); // width 120, height 60
}
Node::Node(NodeFlow *flow,NodeID id) : ParentFlow(flow),id(id)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges);
}
