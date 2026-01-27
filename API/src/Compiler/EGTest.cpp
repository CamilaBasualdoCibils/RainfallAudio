

#include "Compiler/ExecutionGraph.hpp"
#include "Compiler/Instruction/Export/AudioStreamExport.hpp"
#include "Compiler/Instruction/Filters/LowPass.hpp"
#include "Compiler/Instruction/Values/AudioStream.hpp"
#include "Compiler/RainfallCompiler.hpp"
#include "Instruction/Generators/Oscillator.hpp"
#include "Instruction/Instruction.hpp"
#include "Instruction/Values/Scalar.hpp"

void EGTest()
{
    ExecutionGraph eg;

    auto oscillator = eg.AddInstruction<Oscillator>();

    const auto const_var = eg.MakeConstant<Scalar>( 0.5f);
    const auto const_var2 = eg.MakeConstant<Scalar>( 500.0f);

    oscillator.SetInput<Oscillator::Inputs::eAmpltiude>(const_var);
    oscillator.SetInput<Oscillator::Inputs::eFrequency>(const_var2);


    auto low_pass_filter = eg.AddInstruction<LowPass>();

    low_pass_filter.SetInput<LowPass::Inputs::eIn>(
        oscillator.GetOutput<Oscillator::Outputs::eWave>());

    low_pass_filter.SetInput<LowPass::Inputs::ePhase>(
        oscillator.GetOutput<Oscillator::Outputs::ePhase>());

    auto audio_export = eg.AddInstruction<AudioStreamExport>(),
         audio_export_2 = eg.AddInstruction<AudioStreamExport>();

    audio_export.SetInput<AudioStreamExport::Inputs::eWaveOut>(
        oscillator.GetOutput<Oscillator::Outputs::eWave>());
    audio_export_2.SetInput<AudioStreamExport::Inputs::eWaveOut>(
        low_pass_filter.GetOutput<LowPass::Outputs::eOut>());

    std::cout << eg.EmitDot() << std::endl;
};