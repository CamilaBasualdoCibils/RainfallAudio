#pragma once

#include "NodeFlow/Audio/AudioNode.hpp"
#include "NodeFlow/DataTypes/AudioStream.hpp"
#include "NodeFlow/Node.hpp"
#include "NodeFlow/Port.hpp"
#include "NodeFlow/RegisterNode.hpp"
#include "imgui.h"
#include <iostream>

class WaveGeneratorNode : public AudioNode
{
    std::shared_ptr<Port<AudioStream>> OutputPort;
  public:
    WaveGeneratorNode(NodeFlow *nf) : AudioNode(nf)
    {
        OutputPort = AddPort<AudioStream>();
        // AddOutput<AudioStream>("Output");
        // AddInput<AudioStream>("Input");
    }
    ~WaveGeneratorNode() = default;
};
REGISTER_NODE(WaveGeneratorNode, "Wave Generator", "");