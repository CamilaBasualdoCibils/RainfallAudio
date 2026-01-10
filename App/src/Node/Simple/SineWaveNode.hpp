#pragma once

#include "Node/Node.hpp"
#include "Node/RegisterNode.hpp"
#include <iostream>
class SineWaveNode : public Node
{
public:
SineWaveNode() = default;
~SineWaveNode() = default;
    void DrawPreview() override
    {
        std::cout << "SineWaveNode Preview" << std::endl;
    }
    std::string GetTitle() override {return "Sine Wave";}
};
REGISTER_NODE(SineWaveNode, "Sine Wave", "");