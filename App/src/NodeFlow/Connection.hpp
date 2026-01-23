#pragma once
#include "IDs.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <qpoint.h>
#include <variant>
class NodeFlow;

class Connection : public QGraphicsItem
{
    NodeFlow *flow;

  public:
    Connection(NodeFlow *flow, PortRef a, PortRef b) : flow(flow), From(a), To(b)
    {
        setZValue(-1); // behind nodes
        setFlag(ItemIsSelectable);
        setFlag(ItemIsFocusable);
    }

    QRectF boundingRect() const override
    {
        QPointF p1 = GetPortScenePos(From);
        QPointF p2 = GetPortScenePos(To);
        QRectF rect(p1, p2);
        rect = rect.normalized();
        rect.adjust(-10, -10, 10, 10); // give some padding for the curve width
        return rect;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        QPointF p1 = GetPortScenePos(From);
        QPointF p2 = GetPortScenePos(To);

        // Create smooth cubic Bézier curve
        QPainterPath path(p1);

        qreal dx = (p2.x() - p1.x()) * 0.5;
        QPointF c1(p1.x() + dx, p1.y());
        QPointF c2(p2.x() - dx, p2.y());

        path.cubicTo(c1, c2, p2);

        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(Qt::black, 2));
        painter->drawPath(path);
    }
    bool operator<(const Connection &rhs) const
    {
        if (From != rhs.From)
            return From < rhs.From;
        return To < rhs.To;
    }

    bool operator==(const Connection &rhs) const { return From == rhs.From && To == rhs.To; }
    PortRef GetFrom() const { return From; }
    PortRef GetTo() const { return To; }

  private:
    QPointF GetPortScenePos(PortRef p) const;
    PortRef From;
    PortRef To;
};
struct ConnectionHash
{
    std::size_t operator()(const Connection &c) const noexcept
    {
        std::size_t h1 =
            std::hash<NodeID>{}(c.GetFrom().first) ^ (std::hash<PortID>{}(c.GetFrom().second) << 1);
        std::size_t h2 =
            std::hash<NodeID>{}(c.GetTo().first) ^ (std::hash<PortID>{}(c.GetTo().second) << 1);
        return h1 ^ (h2 << 1);
    }
};