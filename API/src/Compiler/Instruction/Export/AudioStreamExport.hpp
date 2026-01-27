#pragma once


#include "Compiler/Instruction/Instruction.hpp"
#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/Instruction/Values/Scalar.hpp"
class AudioStreamExport: public IInstruction
{
  public:

    AudioStreamExport(ExecutionGraph& eg,InstructionID ID) : IInstruction(eg,ID)
    {
        AddInput<AudioStream>("Output");
    }
};