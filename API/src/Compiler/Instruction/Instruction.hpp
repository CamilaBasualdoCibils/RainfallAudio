#pragma once

#include "Variable.hpp"
#include "boost/container/small_vector.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/tag.hpp"
#include "boost/multi_index_container.hpp"
#include <cassert>
#include <cstddef>
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
    template <typename Type, auto Key>
    void DeclareInput(std::string_view name = {})
    {
        using KeyT = decltype(Key);

        InputEntry e;
        e.data_type_index = typeid(Type);
        e.Name = name.empty() ? typeid(KeyT).name() : name;

        e.KeyHash = HashFromKey<Key>();
        assert(!inputs.get<VariableByKey>().contains(e.KeyHash) &&
               "Duplicate keys");
        inputs.insert(e);
    }
    template <typename Type, auto Key>
    void DeclareOutput(std::string_view name = {})
    {
        using KeyT = decltype(Key);

        OutputEntry e;
        e.data_type_index = typeid(Type);
        e.Name = name.empty() ? typeid(KeyT).name() : name;

        e.KeyHash = HashFromKey<Key>();
        assert(!outputs.get<VariableByKey>().contains(e.KeyHash) &&
               "Duplicate keys");

        outputs.insert(e);
    }

  public:
    template <auto Key> static size_t HashFromKey()
    {
        using KeyT = decltype(Key);
        if constexpr (std::is_enum_v<KeyT> || std::is_integral_v<KeyT>)
        {
            return static_cast<size_t>(Key);
        }
        else
        {
            return static_cast<size_t>(typeid(Key).hash_code());
        }
    }
    IInstruction(ExecutionGraph &eg, InstructionID ID) : eg(eg), ID(ID) {}
    struct InputOutputEntry
    {
        std::string Name;
        size_t KeyHash;
        std::optional<std::type_index> data_type_index;
    };
    struct InputEntry : InputOutputEntry
    {
    };
    struct OutputEntry : InputOutputEntry
    {
    };
    void ForEach_Input(std::function<void(const InputEntry &)> lambda)
    {
        for (const auto &input : inputs.get<VariableByKey>())
        {
            lambda(input);
        }
    }
    void ForEach_Output(std::function<void(const OutputEntry &)> lambda)
    {
        for (const auto &output : outputs.get<VariableByKey>())
        {
            lambda(output);
        }
    }
    bool hasInputWithHash(MemberVarHash hash) const {return inputs.get<VariableByKey>().contains(hash);}
    const InputEntry& GetInputFromHash(MemberVarHash hash) const {return *inputs.get<VariableByKey>().find(hash); }
    InstructionID GetID() const { return ID; }
    virtual std::string GetLabel() const = 0;
  private:
    MemberVarHash ReserveInputID() { return NextInputID++; }
    MemberVarHash ReserveOutputID() { return NextOutputID++; }
    MemberVarHash NextInputID = 0, NextOutputID = 0;

    struct VariableByKey
    {
    };
    using InputContainer = boost::multi_index_container<
        InputEntry,
        boost::multi_index::indexed_by<boost::multi_index::hashed_unique<
            boost::multi_index::tag<VariableByKey>,
            boost::multi_index::member<InputOutputEntry, size_t,
                                       &InputOutputEntry::KeyHash>>>>;
    using OutputContainer = boost::multi_index_container<
        OutputEntry,
        boost::multi_index::indexed_by<boost::multi_index::hashed_unique<
            boost::multi_index::tag<VariableByKey>,
            boost::multi_index::member<InputOutputEntry, size_t,
                                       &InputOutputEntry::KeyHash>>>>;
    InputContainer inputs;
    OutputContainer outputs;
};
template <typename T> class Instruction
{

  public:
};