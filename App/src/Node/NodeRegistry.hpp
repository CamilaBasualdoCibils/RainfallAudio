#pragma once

#include "misc/Singleton.hpp"
#include <Node/Node.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index_container.hpp>

class NodeRegistry : public Singleton<NodeRegistry>
{
  public:
    struct NodeEntry
    {
        std::string Name, Icon;
        std::function<std::unique_ptr<Node>()> Create;
    };

  private:
    struct NodeTypeByName
    {
    };

    boost::multi_index_container<
        NodeEntry, boost::multi_index::indexed_by<
                       // non-unique by state
                       boost::multi_index::ordered_non_unique<
                           boost::multi_index::tag<NodeTypeByName>,
                           boost::multi_index::member<NodeEntry, std::string,
                                                      &NodeEntry::Name>>>>
        node_registry;

  public:
    template <typename T>
    bool RegisterNode(const std::string_view Name, const std::string_view Icon)
    {
        NodeEntry entry;
        entry.Name = Name;
        entry.Icon = Icon;
        entry.Create = []() { return std::make_unique<T>(); };
        node_registry.insert(entry);
        return true;
    }
    void ForEach_NodeType(std::function<void(const NodeEntry &)> lambda)
    {
        for (const NodeEntry &entry : node_registry.get<NodeTypeByName>())
        {
            lambda(entry);
        }
    }
};