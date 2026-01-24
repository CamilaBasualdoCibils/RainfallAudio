#pragma once
#include "NodeFlow/IDs.hpp"
#include "NodeFlow/Port.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index_container.hpp>
#include <memory>
class NodeFlow;
class Node : public QGraphicsItem
{
    NodeFlow *ParentFlow;
    NodeID id = -1;
    PortID NextPortID = 0;

  public:
    Node(NodeFlow *flow, NodeID id);

    // Define bounding rectangle (required)
    QRectF boundingRect() const override;

    // Draw the node
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    constexpr NodeID GetID() const { return id; }
    PortID ReservePortID() { return NextPortID++; }
    std::shared_ptr<IPort> GetPort(PortID id)
    {
        const auto &view = ports.get<PortsByID>();
        if (view.contains(id))
        {
            return *view.find(id);
        }
        return nullptr;
    }


  protected:
    template <typename T> std::shared_ptr<Port<T>> AddInputPort()
    {
        std::shared_ptr<Port<T>> port = AddPort<T>();
        port->SetDirection(PortDir::eInput);
        return port;
    }
     template <typename T> std::shared_ptr<Port<T>> AddOutputPort()
    {
        std::shared_ptr<Port<T>> port = AddPort<T>();
        port->SetDirection(PortDir::eOutput);
        return port;
    }
  private:
     template <typename T> std::shared_ptr<Port<T>> AddPort()
    {
        std::shared_ptr<Port<T>> port = std::make_shared<Port<T>>(ParentFlow, id, ReservePortID());
        ports.insert(port);
        port->show();
        port->setParentItem(this);
        return port;
    }

    QString m_title = "Node";
    struct PortsByID
    {
    };
    using PortContainer = boost::multi_index_container<
        std::shared_ptr<IPort>,
        boost::multi_index::indexed_by<
            // Ordered unique index by IPort::GetID()
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<PortsByID>,
                boost::multi_index::const_mem_fun<IPort, PortID, &IPort::GetID>>>>;
    PortContainer ports;
    // std::vector<std::shared_ptr<IPort>> ports;
};
