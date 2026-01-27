#pragma once
#include "Compiler/Instruction/Defines.hpp"
#include <any>
#include <string>
#include <typeindex>
#include "Values/Value.hpp"
struct IVariable
{
    std::string Name;
    std::type_index data_type_index;
    GlobalVariableID GlobalID;
    IVariable(GlobalVariableID id, const std::type_index &type, const std::string_view name)
        : GlobalID(id), Name(name), data_type_index(type)
    {
    }
};

struct IMemberVariable : IVariable
{

    InstructionID CreatorID;
    MemberVarHash memberHash;
    IMemberVariable(GlobalVariableID GlobalID, InstructionID CreatorID, MemberVarHash memberHash,
                    const std::type_index &type, const std::string_view name)
        : IVariable(GlobalID, type, name), CreatorID(CreatorID), memberHash(memberHash) {};
};

struct IConstant : IVariable
{
    IConstant(GlobalVariableID id, const std::type_index &type, const std::string_view name)
        : IVariable(id, type, name)
    {
    }
    virtual const IValue& GetValue() const =0;
};
template <typename DataType> struct Constant : IConstant
{
    DataType value;
    Constant(GlobalVariableID id, const std::type_index &type, const std::string_view name,
             const DataType &value)
        : IConstant(id, type, name), value(value)
    {
    }
    const IValue& GetValue() const override {return value;};
};
