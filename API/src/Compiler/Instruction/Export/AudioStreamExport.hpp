#pragma once

#include "Compiler/Instruction/Instruction.hpp"
#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/Instruction/Values/Scalar.hpp"
class AudioStreamExport : public IInstruction
{
  public:
    enum class Inputs
    {
        eWaveOut
    };
    const static inline std::string Output_VarName = "Output";
    AudioStreamExport(ExecutionGraph &eg, InstructionID ID)
        : IInstruction(eg, ID)
    {
        DeclareInput<AudioStream, Inputs::eWaveOut>("WaveOutput");
    }
    std::string GetLabel() const override { return "AudioStreamExport"; }
};