#pragma once
#include "Compiler/Instruction/Defines.hpp"
#include <any>
#include <string>
#include <typeindex>

struct IValue
{
    std::string Name;
    std::type_index data_type_index;
    GlobalVariableID GlobalID;
    IValue(GlobalVariableID id, const std::type_index &type, const std::string_view name)
        : GlobalID(id), Name(name), data_type_index(type)
    {
    }
};

struct IMemberVariable : IValue
{

    InstructionID CreatorID;
    MemberVarHash memberHash;
    IMemberVariable(GlobalVariableID GlobalID, InstructionID CreatorID, MemberVarHash memberHash,
                    const std::type_index &type, const std::string_view name)
        : IValue(GlobalID, type, name), CreatorID(CreatorID), memberHash(memberHash) {};
};

struct IConstant : IValue
{
    IConstant(GlobalVariableID id, const std::type_index &type, const std::string_view name)
        : IValue(id, type, name)
    {
    }
};
template <typename DataType> struct Constant : IConstant
{
    DataType value;
    Constant(GlobalVariableID id, const std::type_index &type, const std::string_view name,
             const DataType &value)
        : IConstant(id, type, name), value(value)
    {
    }
};
