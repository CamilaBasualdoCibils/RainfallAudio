#pragma once

#include "Node/NodeFlowObject.hpp"
#include "imgui.h"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <cstdint>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>
#include <Node/NodePin.hpp>
class NodeFlow;
class Node : public NodeFlowObject,public std::enable_shared_from_this<Node>
{
    static inline uint32_t nextID = 1;
    const uint32_t ID = nextID++;

  public:
    Node(const NodeFlow &flow) : NodeFlowObject(flow) {}
    void SetPosition(ImVec2 p) { position = {p.x,p.y}; }
    virtual void Draw();

    uint32_t GetInputPinCount() const { return InputPins.size(); }
    uint32_t GetOutputPinCount() const { return OutputPins.size(); }
    std::shared_ptr<INodePin> GetInputPin(uint32_t index)
    {
        const auto &seq = InputPins.get<IndexBySequence>();
        auto it = seq.begin();
        std::advance(it, index);
        if (it != seq.end())
        {
            return *it;
        }
        return nullptr;
    }
    std::shared_ptr<INodePin> GetOutputPin(uint32_t index)
    {
        const auto &seq = OutputPins.get<IndexBySequence>();
        auto it = seq.begin();
        std::advance(it, index);
        if (it != seq.end())
        {
            return *it;
        }
        return nullptr;
    }

  protected:
    template <typename T> NodePin<T> &AddInput(const std::string_view name)
    {
        std::shared_ptr<NodePin<T>> pin = std::make_shared<NodePin<T>>(GetFlow(),name);
        pin->SetParent(this);
        InputPins.push_back(pin);
        return *pin;
    }
    template <typename T> NodePin<T> &AddOutput(const std::string_view name)
    {
        std::shared_ptr<NodePin<T>> pin = std::make_shared<NodePin<T>>(GetFlow(),name);
        OutputPins.push_back(pin);
        pin->SetParent(this);
        return *pin;
    }

  private:
    void DrawPins(NodePinType t);
    struct IndexBySequence
    {
    };
    boost::multi_index_container<std::shared_ptr<INodePin>,
                                 boost::multi_index::indexed_by<boost::multi_index::sequenced<
                                     boost::multi_index::tag<IndexBySequence>>>>
        InputPins;
    boost::multi_index_container<std::shared_ptr<INodePin>,
                                 boost::multi_index::indexed_by<boost::multi_index::sequenced<
                                     boost::multi_index::tag<IndexBySequence>>>>
        OutputPins;
};