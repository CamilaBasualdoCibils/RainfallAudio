#include "NodeFlow.hpp"

std::shared_ptr<Node> NodeFlow::GetNode(NodeID id)
{
    const auto &seq = nodes.get<NodeByID>();
    if (seq.contains(id))
    {
        return *seq.find(id);
    }
    return nullptr;
}
