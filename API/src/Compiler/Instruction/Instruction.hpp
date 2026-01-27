#pragma once


#include "Variable.hpp"
#include "boost/container/small_vector.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/tag.hpp"
#include "boost/multi_index_container.hpp"
#include <cstdint>
#include <memory>
#include <string_view>
#include <typeindex>

#include "Defines.hpp"
class ExecutionGraph;
struct IInstruction
{
    private:
    ExecutionGraph &eg;
    InstructionID ID;
    std::string Name;

  protected:
    template <typename Type> void AddInput(const std::string_view name)
    {
        InputEntry e;
        e.data_type_index = typeid(Type);
        e.Name = name;
        e.InputID = ReserveInputID();
        inputs.push_back(e);
    }
    template <typename Type> void AddOutput(const std::string_view name)
    {
        OutputEntry e;
        e.data_type_index = typeid(Type);
        e.Name = name;
        e.OutputID = ReserveOutputID();
        outputs.push_back(e);
    }

  public:
    IInstruction(ExecutionGraph &eg, InstructionID ID) : eg(eg), ID(ID) {}
    struct InputOutputEntry
    {
        std::string Name;
        std::optional<std::type_index> data_type_index;
    };
    struct InputEntry : InputOutputEntry
    {
        ScopeVariableID InputID;
    };
    struct OutputEntry : InputOutputEntry
    {
        ScopeVariableID OutputID;
    };
    void ForEach_Input(std::function<void(const InputEntry &)> lambda)
    {
        for (const auto &input : inputs.get<VariableByName>())
        {
            lambda(input);
        }
    }
    void ForEach_Output(std::function<void(const OutputEntry &)> lambda)
    {
        for (const auto &output : outputs.get<VariableByName>())
        {
            lambda(output);
        }
    }
    InstructionID GetID() const {return ID;}
  private:
    ScopeVariableID ReserveInputID() { return NextInputID++; }
    ScopeVariableID ReserveOutputID() { return NextOutputID++; }
    ScopeVariableID NextInputID = 0,NextOutputID = 0;

    struct VariableByName
    {
    };
    using InputContainer =
        boost::multi_index_container<InputEntry,
                                     boost::multi_index::indexed_by<boost::multi_index::sequenced<
                                         boost::multi_index::tag<VariableByName>
                                         >>>;
    using OutputContainer =
        boost::multi_index_container<OutputEntry,
                                     boost::multi_index::indexed_by<boost::multi_index::sequenced<
                                         boost::multi_index::tag<VariableByName>
                                         >>>;
    InputContainer inputs;
    OutputContainer outputs;
};
template <typename T> class Instruction
{

  public:
};