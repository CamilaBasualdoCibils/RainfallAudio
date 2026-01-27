#pragma once
#include "Compiler/Instruction/Defines.hpp"
#include <any>
#include <string>
#include <typeindex>

struct IValue
{
    std::string Name;
    std::type_index data_type_index;
    IValue(const std::type_index &type, const std::string_view name)
        : Name(name), data_type_index(type)
    {
    }
    const std::string &getName() const { return Name; }
};

struct IMemberVariable : IValue
{

    InstructionID CreatorID;
    ScopeVariableID ID;
    IMemberVariable(InstructionID CreatorID, ScopeVariableID ID, const std::type_index &type,
                    const std::string_view name)
        : IValue(type, name), CreatorID(CreatorID), ID(ID) {};
    InstructionID getCreatorID() const { return CreatorID; }
    ScopeVariableID getID() const { return ID; }
};

struct IConstant : IValue
{
    ScopeVariableID ID;
    IConstant(ScopeVariableID id, const std::type_index &type, const std::string_view name)
        : IValue(type, name), ID(id)
    {
    }
};
template <typename DataType> struct Constant : IConstant
{
    DataType value;
    Constant(ScopeVariableID id, const std::type_index &type, const std::string_view name,
             const DataType &value)
        : IConstant(id, type, name), value(value)
    {
    }
};
