

#include "Compiler/ExecutionGraph.hpp"
#include "Compiler/Instruction/Export/AudioStreamExport.hpp"
#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/RainfallCompiler.hpp"
#include "Instruction/Generator/Oscillator.hpp"
#include "Instruction/Instruction.hpp"
#include "Instruction/Values/Scalar.hpp"

void EGTest()
{
    ExecutionGraph eg;

    // add oscillator instruction
    auto oscillator = eg.AddInstruction<Oscillator>();

    //// set its input to a constant
    const auto const_var = eg.MakeConstant<Scalar>("a", 0.5f);

    oscillator.SetInput<Oscillator::Inputs::eAmpltiude>(const_var);

    //// Add Audio Stream Export instruction
    ExecutionGraph::inst_iterator<AudioStreamExport> audio_export =
        eg.AddInstruction<AudioStreamExport>();

    //
    // set its input to the output of Oscillator
    auto oscillator_output = oscillator.GetOutput<Oscillator::Outputs::eWave>();

    audio_export.SetInput<AudioStreamExport::Inputs::eWaveOut>(oscillator_output);
    //
    // compile
    // RainfallCompiler::Get().Compile(eg.EmitLLVM());
    std::cout << eg.EmitDot() << std::endl;
};