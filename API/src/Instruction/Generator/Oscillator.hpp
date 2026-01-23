#pragma once

#include "Instruction/Instruction.hpp"
#include "Instruction/Values/AudioStream.hpp"
#include "Instruction/Values/Scalar.hpp"
class Oscillator : public IInstruction
{
    Oscillator() : IInstruction("Oscillator")
    {
        AddInput<Scalar>("Frequency");
        AddInput<Scalar>("Amplitude");
        AddOutput<AudioStream>("Wave");
    }
};
