#pragma once

#include "NodeFlow/Audio/AudioNode.hpp"
#include "NodeFlow/DataTypes/AudioStream.hpp"
#include "NodeFlow/Node.hpp"
#include "NodeFlow/Port.hpp"
#include "NodeFlow/RegisterNode.hpp"
#include "imgui.h"
#include <iostream>
#include <qpoint.h>

class WaveGeneratorNode : public AudioNode
{
    std::shared_ptr<Port<AudioStream>> OutputPort;
  public:
    WaveGeneratorNode(NodeFlow *flow,NodeID id) : AudioNode(flow,id)
    {
        OutputPort = AddPort<AudioStream>();
        OutputPort->setPos(QPointF{-boundingRect().x(),0});
        OutputPort->SetDirection(PortDir::eOutput);
        // AddOutput<AudioStream>("Output");
        // AddInput<AudioStream>("Input");
    }
    ~WaveGeneratorNode() = default;
};
REGISTER_NODE(WaveGeneratorNode, "Wave Generator", "");