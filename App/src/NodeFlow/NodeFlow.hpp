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

    void GetConnectionsAtPort(PortRef p,
                              std::vector<std::shared_ptr<const Connection>> &out_connections) const
    {
        out_connections.clear();
        for (const auto &seq = connections.get<ConnectionByFrom>();
             const std::shared_ptr<const Connection> conn : seq)
        {
            const auto range = seq.equal_range(p);
            for (auto it = range.first; it != range.second; it++)
            {
                out_connections.push_back(*it);
            }
        }
        for (const auto &seq = connections.get<ConnectionByTo>();
             const std::shared_ptr<const Connection> conn : seq)
        {
            const auto range = seq.equal_range(p);
            for (auto it = range.first; it != range.second; it++)
            {
                out_connections.push_back(*it);
            }
        }
    }

    void DisconnectAllAtPort(PortRef p)
    {
        auto &from_index = connections.get<ConnectionByFrom>();
        auto from_range = from_index.equal_range(p);
        for (auto it = from_range.first; it != from_range.second;)
        {
            it = from_index.erase(it); // safe: returns next iterator
        }

        auto &to_index = connections.get<ConnectionByTo>();
        auto to_range = to_index.equal_range(p);
        for (auto it = to_range.first; it != to_range.second;)
        {
            it = to_index.erase(it); // safe: returns next iterator
        }
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