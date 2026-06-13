#pragma once
#include "Compiler/Instruction/Defines.hpp"
#include "Instruction/Instruction.hpp"
#include "Instruction/Variable.hpp"
#include "boost/multi_index/composite_key.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/tag.hpp"
#include "boost/multi_index_container_fwd.hpp"
#include <cassert>
#include <format>
#include <memory>
#include <string>
#include <string_view>

#include <iostream>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
class ExecutionGraph;

class ExecutionGraph
{

  public:
    class value_iterator
    {
      public:
        value_iterator(ExecutionGraph &eg, std::shared_ptr<IVariable> d)
            : eg(eg), data(d)
        {
        }
        GlobalVariableID GetGlobalID() const { return data->GlobalID; }

      protected:
        ExecutionGraph &eg;

      private:
        std::shared_ptr<IVariable> data;
    };
    class i_inst_iterator
    {

      public:
        i_inst_iterator(ExecutionGraph &eg, std::shared_ptr<IInstruction> d)
            : eg(eg), data(d)
        {
        }
        IInstruction *operator->() { return data.get(); };
        IInstruction const *operator->() const { return data.get(); };
        template <auto Key> void SetInput(value_iterator v)
        {
            eg.internal_CreateReference(v.GetGlobalID(), data->GetID(),
                                        IInstruction::HashFromKey<Key>());
        }
        template <auto Key> value_iterator GetOutput()
        {
            return eg.internal_GetMemberVar(data->GetID(),
                                            IInstruction::HashFromKey<Key>());
        }

      protected:
        ExecutionGraph &eg;

      private:
        std::shared_ptr<IInstruction> data;
    };
    template <typename T> class inst_iterator : public i_inst_iterator
    {
      public:
        inst_iterator(ExecutionGraph &eg, std::shared_ptr<T> d)
            : i_inst_iterator(eg, d)
        {
        }
        T *operator->() { return tdata.get(); };
        T const *operator->() const { return tdata.get(); };

      private:
        std::shared_ptr<T> tdata;
    };
    friend i_inst_iterator;
    friend value_iterator;
    ExecutionGraph() {}
    template <typename T> [[nodiscard]] inst_iterator<T> AddInstruction()
    {
        std::shared_ptr<T> orig_ins =
            std::make_shared<T>(*this, ReserveInstructionID());
        std::shared_ptr<IInstruction> ins = orig_ins;
        instructions.insert(ins);
        ins->ForEach_Output(
            [&](const IInstruction::OutputEntry &e)
            {
                std::shared_ptr<IMemberVariable> val =
                    std::make_shared<IMemberVariable>(
                        ReserveVariableID(), ins->GetID(), e.KeyHash,
                        e.data_type_index.value(), e.Name);
                OutputMemberVariables.insert(val);
            });
        return inst_iterator<T>(*this, orig_ins);
    }

    template <typename T>
    [[nodiscard]] value_iterator MakeConstant(const std::string_view name, T v)
    {
        std::shared_ptr<Constant<T>> con = std::make_shared<Constant<T>>(
            ReserveVariableID(), typeid(T), name, v);
        constants.insert(con);

        return value_iterator(*this, con);
    }
    template <typename T> [[nodiscard]] value_iterator MakeConstant(T v)
    {
        return MakeConstant("", v);
    }

    std::string EmitDot();
    std::unique_ptr<llvm::Module> EmitLLVM(llvm::LLVMContext &Context);

  private:
    [[nodiscard]] value_iterator internal_GetMemberVar(InstructionID ins_id,
                                                       MemberVarHash var_id);
    [[nodiscard]] value_iterator internal_GetGlobalVar(GlobalVariableID id);
    [[nodiscard]] value_iterator
    internal_GetMemberVar(InstructionID ins_id, const std::string_view str);
    void internal_CreateReference(GlobalVariableID from_global_var_id,
                                  InstructionID to_instruction_id,
                                  MemberVarHash to_instruction_member_var_hash);
    InstructionID NextInstructionID = 0;
    GlobalVariableID NextVariableID = 0;
    InstructionID ReserveInstructionID() { return NextInstructionID++; }
    GlobalVariableID ReserveVariableID() { return NextVariableID++; }

    struct ByInstructionID
    {
    };
    struct ByGlobalID
    {
    };
    struct ByInstruction_MemberHash_Pair
    {
    };
    struct ByType
    {
    };
    using InstructionContainer = boost::multi_index_container<
        std::shared_ptr<IInstruction>,
        boost::multi_index::indexed_by<boost::multi_index::ordered_unique<
            boost::multi_index::tag<ByInstructionID>,
            boost::multi_index::const_mem_fun<IInstruction, InstructionID,
                                              &IInstruction::GetID>
            // boost::multi_index::member<IInstruction,
            // std::type_index,&InstructionInstance::instruction_type>
            >>>;

    using ConstantContainer = boost::multi_index_container<
        std::shared_ptr<IConstant>,
        boost::multi_index::indexed_by<boost::multi_index::ordered_unique<
            boost::multi_index::tag<ByGlobalID>,
            boost::multi_index::member<IVariable, GlobalVariableID,
                                       &IVariable::GlobalID>>>>;

    using MemberVarContainer = boost::multi_index_container<
        std::shared_ptr<IMemberVariable>, // what it stores
        boost::multi_index::indexed_by<

            boost::multi_index::ordered_unique<
                boost::multi_index::tag<ByGlobalID>,
                boost::multi_index::member<IVariable, GlobalVariableID,
                                           &IVariable::GlobalID>>,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<ByInstructionID>,
                boost::multi_index::member<IMemberVariable, InstructionID,
                                           &IMemberVariable::CreatorID>>,

            boost::multi_index::hashed_unique<
                boost::multi_index::tag<ByInstruction_MemberHash_Pair>,
                boost::multi_index::composite_key<
                    IMemberVariable,
                    boost::multi_index::member<IMemberVariable, InstructionID,
                                               &IMemberVariable::CreatorID>,
                    boost::multi_index::member<
                        IMemberVariable, MemberVarHash,
                        &IMemberVariable::memberHash>>>>>;
    struct IReference
    {
        // From
        GlobalVariableID FromVarGlobalID;
        // To
        InstructionID ToInstructionID;
        MemberVarHash ToInstructionMemberHash;
    };
    struct ReferenceByFromGlobalID
    {
    };
    using ReferenceContainer = boost::multi_index_container<
        IReference,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_non_unique<
                boost::multi_index::tag<ReferenceByFromGlobalID>,
                boost::multi_index::member<IReference, GlobalVariableID,
                                           &IReference::FromVarGlobalID>>,
            boost::multi_index::sequenced<>>>;

    InstructionContainer instructions;
    ConstantContainer constants;
    MemberVarContainer OutputMemberVariables;
    ReferenceContainer references;
};
class SubGraph : public IInstruction, public ExecutionGraph
{
};