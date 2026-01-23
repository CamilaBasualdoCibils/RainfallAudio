#pragma once


#include "NodeFlow/IDs.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "misc/Singleton.hpp"
#include <NodeFlow/Node.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index_container.hpp>
#include <memory>
#include <typeindex>
class NodeFlow;
class NodeRegistry : public Singleton<NodeRegistry>
{
  public:
    struct NodeEntry
    {
        using CreateFunc = std::function<std::shared_ptr<Node>(NodeFlow *,NodeID)>;
        NodeEntry(const std::string_view Name, const std::string_view Icon,
                  std::type_index type_index, CreateFunc createFunc)
            : Name(Name), Icon(Icon), type_index(type_index), Create(createFunc)
        {
        }
        std::string Name, Icon;
        std::type_index type_index;
        CreateFunc Create;
    };

  private:
    struct NodeTypeByName
    {
    };
    struct NodeTypeByTypeIndex;

    boost::multi_index_container<
        NodeEntry,
        boost::multi_index::indexed_by<
            // non-unique by state
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<NodeTypeByName>,
                boost::multi_index::member<NodeEntry, std::string, &NodeEntry::Name>>,
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<NodeTypeByTypeIndex>,
                boost::multi_index::member<NodeEntry, std::type_index, &NodeEntry::type_index>>>>
        node_registry;

  public:
    template <typename T>
    bool RegisterNode(const std::string_view Name, const std::string_view Icon)
    {
        NodeEntry entry =
            NodeEntry(Name, Icon, typeid(T), [](NodeFlow *nf,NodeID id) { return std::make_shared<T>(nf,id); });

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
    template <typename T> std::optional<NodeEntry> GetNodeEntryFromType()
    {
        const auto &Seq = node_registry.get<NodeTypeByTypeIndex>();
        if (Seq.contains(typeid(T)))
        {
            
            return *Seq.find(typeid(T));
        }
        else
        {
            return std::nullopt;
        }
    }
};