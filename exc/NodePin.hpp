#pragma once

#include "Node/NodeFlowObject.hpp"
#include <memory>
enum class NodePinType
{
    eInput,
    eOutput
};

class INodePin : public NodeFlowObject
{
    std::string Name;
    std::type_index type;

  public:
    INodePin(const NodeFlow &nf,
             const std::string_view _name, std::type_index type)
        : NodeFlowObject(nf), Name(_name), type(type)
    {
    }
    void Draw();
    ~INodePin() override = default;
};

template <typename T> struct NodePin : public INodePin
{

    NodePin(const NodeFlow &nf,
            const std::string_view _name)
        : INodePin(nf, _name, typeid(T))
    {
    }
    ~NodePin() override = default;
};