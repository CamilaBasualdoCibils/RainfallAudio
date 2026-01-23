#pragma once
#include "NodeFlow/NodeRegistry.hpp"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <qgraphicsscene.h>
class Node;
class NodeFlow : public QGraphicsScene
{

    public:
    NodeFlow(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    template <typename T>
    void CreateNode()
    {
        NodeRegistry::Get().GetNodeEntryFromType<T>();
    }
    void AddNode(std::shared_ptr<Node> node)
    {
        nodes.push_back(node);
        addItem(node.get());
    }

    private:
        boost::multi_index_container<
        std::shared_ptr<Node>,
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<>
            // boost::multi_index::hashed_non_unique<boost::multi_index::tag<NodeByType>,
            // NodeTypeExtractor>
            >>
        nodes;
};