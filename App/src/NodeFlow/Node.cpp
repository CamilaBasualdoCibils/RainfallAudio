#include "Node.hpp"
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Background
    painter->setBrush(isSelected() ? Qt::yellow : Qt::cyan);
    painter->setPen(QPen(Qt::blue, 2));
    painter->drawRoundedRect(boundingRect(), 5, 5);

    // Title text
    painter->setPen(Qt::black);
    painter->drawText(boundingRect().adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignTop, m_title);
}
QRectF Node::boundingRect() const
{
    return QRectF(0, 0, 120, 250); // width 120, height 60
}
Node::Node(NodeFlow *flow) : ParentFlow(flow)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges);
}
