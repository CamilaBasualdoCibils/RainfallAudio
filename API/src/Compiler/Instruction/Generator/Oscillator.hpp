#pragma once
#include "Compiler/Instruction/Instruction.hpp"

#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/Instruction/Values/Scalar.hpp"
class Oscillator : public IInstruction
{
  public:

    Oscillator(ExecutionGraph& eg,InstructionID ID) : IInstruction(eg,ID)
    {
        AddInput<Scalar>("Frequency");
        AddInput<Scalar>("Amplitude");
        AddOutput<AudioStream>("Wave");
    }
};
