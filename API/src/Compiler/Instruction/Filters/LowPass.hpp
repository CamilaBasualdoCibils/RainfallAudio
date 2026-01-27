#pragma once

#include "Compiler/Instruction/Instruction.hpp"
#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/Instruction/Values/Scalar.hpp"
class LowPass : public IInstruction
{
    public:
    enum class Inputs
    {
        eIn,ePhase
    };
    enum class Outputs
    {
        eOut
    };
    LowPass(ExecutionGraph &eg, InstructionID ID) : IInstruction(eg, ID)
    {
        DeclareInput<AudioStream, Inputs::eIn>("In");
        DeclareInput<Scalar, Inputs::ePhase>("Phase");
        DeclareOutput<AudioStream, Outputs::eOut>("Out");
    }
    std::string GetLabel() const override { return "Low-Pass"; }
};