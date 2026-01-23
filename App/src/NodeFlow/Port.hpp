#pragma once

#include <qgraphicsitem.h>
class Node;
class IPort : public QGraphicsItem
{
    protected:
    Node* parentNode;
    public:
    IPort(Node *parentNode);
    QRectF boundingRect() const override;

    // Draw the node
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

template <typename t>
class Port : public IPort
{
     public:
    Port(Node *parentNode):IPort(parentNode){}
};