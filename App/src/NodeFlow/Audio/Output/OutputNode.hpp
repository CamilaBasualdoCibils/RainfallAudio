#pragma once

#include "NodeFlow/Audio/AudioNode.hpp"
#include "NodeFlow/DataTypes/AudioStream.hpp"
#include "NodeFlow/Port.hpp"
#include "NodeFlow/RegisterNode.hpp"
#include <qpoint.h>
class OutputNode : public AudioNode
{
    std::shared_ptr<Port<AudioStream>> input;

  public:
    OutputNode(NodeFlow *flow, NodeID id) : AudioNode(flow, id)
    {

        input = AddInputPort<AudioStream>();
        input->SetDirection(PortDir::eInput);
        input->setPos(QPointF{boundingRect().x(), 0});
    }
};
REGISTER_NODE(OutputNode, "Output Node", "");