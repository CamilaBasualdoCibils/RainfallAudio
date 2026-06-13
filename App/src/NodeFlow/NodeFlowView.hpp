#pragma once
#include "NodeFlow/IDs.hpp"
#include "NodeFlow/NodeFlow.hpp"
#include "NodeFlow/Port.hpp"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTimer>
#include <qdebug.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpoint.h>

class NodeFlowView : public QGraphicsView
{
    NodeFlow *parentFlow;

  public:
    NodeFlowView(NodeFlow *nodeFlow, QWidget *parent = nullptr)
        : QGraphicsView(nodeFlow, parent), parentFlow(nodeFlow)
    {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::NoDrag);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        connect(&m_zoomTimer, &QTimer::timeout, this, &NodeFlowView::updateZoom);
        m_zoomTimer.start(16); // roughly 60 FPS
    }

  protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            QPointF scenePos = mapToScene(event->pos());

            QGraphicsItem *item = scene()->itemAt(scenePos, transform());
            if (item)
            {
                if (IPort* port = dynamic_cast<IPort *>(item);port)
                {
                    qDebug() << "Port Clicked";
                    NodeInitialConnection = port;
                }
            }
        }
        else if (event->button() == Qt::MiddleButton)
        {
            m_lastPanPoint = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
        else if (event->button() == Qt::RightButton)
        {
            ShowNodesContextMenu(event->pos());
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        QPointF scenePos = mapToScene(event->pos());
        if (!m_lastPanPoint.isNull())
        {
            QPointF delta = mapToScene(m_lastPanPoint) - scenePos;
            translate(delta.x(), delta.y());
            m_lastPanPoint = event->pos();
        }
        if (NodeInitialConnection)
        {
            DragLineEnd = scenePos;
            viewport()->update();
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            if (NodeInitialConnection)
            {
                QPointF scenePos = mapToScene(event->pos());
                QGraphicsItem *item = scene()->itemAt(scenePos, transform());
                if (item)
                {
                    if (IPort* port = dynamic_cast<IPort *>(item);port)
                    {
                        if (port->GetDirection() == PortDir::eInput)
                        {
                            parentFlow->DisconnectAllAtPort(port->GetRef());
                        }
                        else
                        {
                            parentFlow->DisconnectAllAtPort(NodeInitialConnection->GetRef());
                        }
                        parentFlow->AddConnection(PortRef{port->GetParentID(),port->GetID()},PortRef{NodeInitialConnection->GetParentID(),NodeInitialConnection->GetID()});
                    }
                }
                NodeInitialConnection = nullptr;
                viewport()->update();
            }
        }
        else if (event->button() == Qt::MiddleButton)
        {
            m_lastPanPoint = QPoint();
            setCursor(Qt::ArrowCursor);
        }
        QGraphicsView::mouseReleaseEvent(event);
    }

    void wheelEvent(QWheelEvent *event) override
    {
        constexpr double zoomFactor = 1.03;
        if (event->angleDelta().y() > 0)
            m_targetScale *= zoomFactor;
        else
            m_targetScale /= zoomFactor;

        // Clamp zoom
        m_targetScale = std::clamp(m_targetScale, 0.1, 10.0);
    }

    void drawBackground(QPainter *painter, const QRectF &rect) override
    {
        return;
        // Draw a grid
        constexpr int gridSize = 50;
        QPen pen(Qt::lightGray);
        pen.setWidth(1);
        painter->setPen(pen);

        // Vertical lines
        int left = int(rect.left()) - (int(rect.left()) % gridSize);
        int right = int(rect.right());
        for (int x = left; x < right; x += gridSize)
            painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));

        // Horizontal lines
        int top = int(rect.top()) - (int(rect.top()) % gridSize);
        int bottom = int(rect.bottom());
        for (int y = top; y < bottom; y += gridSize)
            painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    }

    void drawForeground(QPainter *painter, const QRectF &rect) override
    {
        if (NodeInitialConnection)
        {
            painter->setPen(QPen(Qt::yellow, 2, Qt::DashLine));
            QPointF start = NodeInitialConnection->sceneBoundingRect().center();
            painter->drawLine(start, DragLineEnd);
        }
    }

  private:
    void updateZoom()
    {
        // Smooth interpolation
        constexpr double smoothing = 0.4; // smaller = slower, larger = snappier
        double delta = m_targetScale / m_currentScale;
        if (std::abs(delta - 1.0) > 0.001)
        {
            double factor = 1.0 + (delta - 1.0) * smoothing;
            scale(factor, factor);
            m_currentScale *= factor;
        }
    }
    void ShowNodesContextMenu(const QPoint &pos);
    QPoint m_lastPanPoint;
    double m_targetScale = 1.0;  // Zoom we want
    double m_currentScale = 1.0; // Actual current zoom
    QTimer m_zoomTimer;

    IPort *NodeInitialConnection = nullptr;
    QPointF DragLineEnd;
};