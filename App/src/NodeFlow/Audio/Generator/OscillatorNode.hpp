#pragma once

#include "NodeFlow/Audio/AudioNode.hpp"
#include "NodeFlow/DataTypes/AudioStream.hpp"
#include "NodeFlow/Node.hpp"
#include "NodeFlow/Port.hpp"
#include "NodeFlow/RegisterNode.hpp"
#include "imgui.h"
#include <iostream>
#include <qpoint.h>

class OscillatorNode : public AudioNode
{
    std::shared_ptr<Port<AudioStream>> OutputPort;

  public:
    OscillatorNode(NodeFlow *flow, NodeID id) : AudioNode(flow, id)
    {
        OutputPort = AddOutputPort<AudioStream>();
        OutputPort->setPos(QPointF{-boundingRect().x(), 0});
        // AddOutput<AudioStream>("Output");
        // AddInput<AudioStream>("Input");
    }
    ~OscillatorNode() = default;
};
REGISTER_NODE(OscillatorNode, "Oscillator", "");