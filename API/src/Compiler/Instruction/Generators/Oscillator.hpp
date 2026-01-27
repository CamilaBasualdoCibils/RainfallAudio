#pragma once
#include "Compiler/Instruction/Instruction.hpp"

#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/Instruction/Values/Scalar.hpp"
#include <cstdint>
class Oscillator : public IInstruction
{
  public:
    enum class Inputs : uint16_t
    {
        eFrequency,
        eAmpltiude
    };
    enum class Outputs : uint16_t
    {
        eWave,
        ePhase
    };
    Oscillator(ExecutionGraph &eg, InstructionID ID) : IInstruction(eg, ID)
    {
        DeclareInput<Scalar, Inputs::eFrequency>("Frequency");
        DeclareInput<Scalar, Inputs::eAmpltiude>("Amplitude");
        DeclareOutput<AudioStream, Outputs::eWave>("Wave Output");
        DeclareOutput<Scalar, Outputs::ePhase>("Phase");
    }
    std::string GetLabel() const override { return "Oscillator"; }
};
