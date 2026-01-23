#include "Port.hpp"
#include "NodeFlow/Node.hpp"
#include <QPainter>
#include <QWidget>

IPort::IPort(Node *parentNode) : QGraphicsItem(parentNode), parentNode(parentNode) {}
void IPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Background
    painter->setBrush(isSelected() ? Qt::yellow : Qt::cyan);
    painter->setPen(QPen(Qt::blue, 2));
    painter->drawRoundedRect(boundingRect(), 5, 5);

    // Title text
    painter->setPen(Qt::black);
    painter->drawText(boundingRect().adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignTop, "ass");
}
QRectF IPort::boundingRect() const
{
    return QRectF(0, 0, 10, 10); // width 120, height 60
}
