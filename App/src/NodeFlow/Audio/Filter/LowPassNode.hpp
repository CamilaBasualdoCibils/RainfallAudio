#pragma once


#include "NodeFlow/Audio/AudioNode.hpp"
#include "NodeFlow/DataTypes/AudioStream.hpp"
#include "NodeFlow/RegisterNode.hpp"
class LowPassNode : public AudioNode
{

    public:
    LowPassNode(NodeFlow *flow,NodeID id):AudioNode(flow,id) {
        const auto edge = boundingRect().x();
        AddInputPort<AudioStream>()->setPos(edge,0);
        AddOutputPort<AudioStream>()->setPos(-edge,0);
    }
};
REGISTER_NODE(LowPassNode, "Low Pass", "");