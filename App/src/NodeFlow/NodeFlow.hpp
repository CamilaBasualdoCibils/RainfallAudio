#pragma once
#include "NodeFlow/Connection.hpp"
#include "NodeFlow/IDs.hpp"
#include "NodeFlow/NodeRegistry.hpp"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <qgraphicsscene.h>

class Node;
class NodeFlow : public QGraphicsScene
{
    NodeID nextNodeID = 0;

  public:
    NodeFlow(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    template <typename T> void CreateNode() { NodeRegistry::Get().GetNodeEntryFromType<T>(); }
    void AddNode(std::shared_ptr<Node> node)
    {
        nodes.insert(node);
        node->show();
        addItem(node.get());
    }
    // void AddConnection(std::shared_ptr<IPort>)
    NodeID ReserveNodeID() { return nextNodeID++; }

    std::shared_ptr<Node> GetNode(NodeID id);
    std::shared_ptr<IPort> GetPort(PortRef p) { return GetNode(p.first)->GetPort(p.second); }
    void AddConnection(PortRef a, PortRef b)
    {
        std::shared_ptr<Connection> conn = std::make_shared<Connection>(this, a, b);
        connections.insert(conn);
        conn->show();
        addItem(conn.get());
    }

  private:
    struct ConnectionByFrom
    {
    };
    struct ConnectionByTo
    {
    };
    using ConnectionContainer = boost::multi_index_container<
        std::shared_ptr<Connection>,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_non_unique<
                boost::multi_index::tag<ConnectionByFrom>,
                boost::multi_index::const_mem_fun<Connection, PortRef, &Connection::GetFrom>>,
            boost::multi_index::hashed_non_unique<
                boost::multi_index::tag<ConnectionByTo>,
                boost::multi_index::const_mem_fun<Connection, PortRef, &Connection::GetTo>>>>;
    ConnectionContainer connections;

    struct NodeByID
    {
    };
    using NodeContainer = boost::multi_index_container<
        std::shared_ptr<Node>, boost::multi_index::indexed_by<boost::multi_index::ordered_unique<
                                   boost::multi_index::tag<NodeByID>,
                                   boost::multi_index::const_mem_fun<Node, NodeID, &Node::GetID>>>>;

    NodeContainer nodes;

    // std::unordered_set<std::pair<std::shared_ptr<IPort>, std::shared_ptr<IPort>>> connections;
};