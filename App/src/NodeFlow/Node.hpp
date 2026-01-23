#pragma once
#include "NodeFlow/Port.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <memory>
class NodeFlow;
class Node : public QGraphicsItem
{
    NodeFlow *ParentFlow;

  public:
    Node(NodeFlow *flow);

    // Define bounding rectangle (required)
    QRectF boundingRect() const override;

    // Draw the node
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

  protected:
    template <typename T>
    std::shared_ptr<Port<T>> AddPort()
    {
      std::shared_ptr<Port<T>> port = std::make_shared<Port<T>>(this);
      ports.push_back(port);
      port->show();
      return port;
    }
  private:
    QString m_title = "Node";
    std::vector<std::shared_ptr<IPort>> ports;
};
