#pragma once

#include "NodeFlow/IDs.hpp"
#include <qgraphicsitem.h>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
class Node;
class NodeFlow;
enum class PortDir
{
    eInput,
    eOutput
};
class IPort : public QGraphicsItem
{
    PortDir dir = PortDir::eInput;
    PortID ID;
    NodeID ParentID;
    NodeFlow* flow;
  protected:
    Node *parentNode;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        // Your click logic here
        qDebug() << "Clicked!";

        event->accept();   // 🔥 CONSUMES the click
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        event->accept();
    }

  public:
    IPort(NodeFlow *flow, NodeID parentID,PortID ID);
    QRectF boundingRect() const override;
    void SetDirection(PortDir d) { dir = d; }
    PortDir GetDirection() const { return dir; }
    PortID GetID() const {return ID;}
    NodeID GetParentID() const {return ParentID;}
    // Draw the node
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

template <typename t> class Port : public IPort
{
  public:
    Port(NodeFlow *flow, NodeID parentID,PortID ID) : IPort(flow,parentID,ID) {}
};