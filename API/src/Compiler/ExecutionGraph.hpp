#pragma once
#include "Compiler/Instruction/Defines.hpp"
#include "Instruction/Instruction.hpp"
#include "Instruction/Variable.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/detail/adjacency_list.hpp"
#include "boost/graph/graph_selectors.hpp"
#include "boost/multi_index/composite_key.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/tag.hpp"
#include "boost/multi_index_container_fwd.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <taskflow/taskflow.hpp>
#include <utility>
class ExecutionGraph;

class ExecutionGraph
{

  public:
    class value_iterator
    {
      public:
        value_iterator(ExecutionGraph &eg, std::shared_ptr<IValue> d) : eg(eg), data(d) {}

      protected:
        ExecutionGraph &eg;

      private:
        std::shared_ptr<IValue> data;
    };
    class i_inst_iterator
    {

      public:
        i_inst_iterator(ExecutionGraph &eg, std::shared_ptr<IInstruction> d) : eg(eg), data(d) {}
        IInstruction *operator->() { return data.get(); };
        IInstruction const *operator->() const { return data.get(); };
        void SetInput(std::string_view a, value_iterator v) {
          
        }
        value_iterator GetOutput(std::string_view varName)
        {
            return eg.internal_GetMemberVar(data->GetID(), varName);
        }

      protected:
        ExecutionGraph &eg;

      private:
        std::shared_ptr<IInstruction> data;
    };
    template <typename T> class inst_iterator : public i_inst_iterator
    {
      public:
        inst_iterator(ExecutionGraph &eg, std::shared_ptr<T> d) : i_inst_iterator(eg, d) {}
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
        std::shared_ptr<T> orig_ins = std::make_shared<T>(*this, ReserveInstructionID());
        std::shared_ptr<IInstruction> ins = orig_ins;
        instructions.push_back(ins);
        ins->ForEach_Output(
            [&](const IInstruction::OutputEntry &e)
            {
                std::shared_ptr<IMemberVariable> val = std::make_shared<IMemberVariable>(
                    ins->GetID(), ReserveVariableID(), e.data_type_index.value(), e.Name);
                memberVariables.insert(val);
            });
        return inst_iterator<T>(*this, orig_ins);
    }

    template <typename T>
    [[nodiscard]] value_iterator MakeConstant(const std::string_view name, T v)
    {
        std::shared_ptr<Constant<T>> con =
            std::make_shared<Constant<T>>(ReserveVariableID(), typeid(T), name, v);
        constants.push_back(con);
        return value_iterator(*this, con);
    }

    std::string EmitDot() { return ""; }

  private:
    [[nodiscard]] value_iterator internal_GetMemberVar(InstructionID ins_id,
                                                       ScopeVariableID var_id);
    [[nodiscard]] value_iterator internal_GetMemberVar(InstructionID ins_id,
                                                       const std::string_view str);
    InstructionID NextInstructionID = 0;
    ScopeVariableID NextVariableID = 0;
    InstructionID ReserveInstructionID() { return NextInstructionID++; }
    ScopeVariableID ReserveVariableID() { return NextVariableID++; }

    struct ByInstructionID
    {
    };
    struct ByScopeID
    {
    };
    struct ByType
    {
    };
    using InstructionContainer =
        boost::multi_index_container<std::shared_ptr<IInstruction>,
                                     boost::multi_index::indexed_by<boost::multi_index::sequenced<
                                         boost::multi_index::tag<ByType>
                                         // boost::multi_index::member<IInstruction,
                                         // std::type_index,&InstructionInstance::instruction_type>
                                         >>>;

    using ConstantContainer = boost::multi_index_container<
        std::shared_ptr<IConstant>,
        boost::multi_index::indexed_by<boost::multi_index::sequenced<>>>;

    using MemberVarContainer = boost::multi_index_container<
        std::shared_ptr<IMemberVariable>,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_non_unique<
                boost::multi_index::tag<ByInstructionID>,
                boost::multi_index::member<IMemberVariable, InstructionID,
                                           &IMemberVariable::CreatorID>>,
            boost::multi_index::hashed_non_unique<
                boost::multi_index::tag<ByScopeID>,
                boost::multi_index::member<IMemberVariable, ScopeVariableID, &IMemberVariable::ID>>

            >>;

    struct IReference
    {
        // From
        ScopeVariableID VariableID;
        // To
        InstructionID instructionID;
        MemberVariableID instructionInputID;
    };
    using ReferenceContainer = boost::multi_index_container<
        IReference, boost::multi_index::indexed_by<boost::multi_index::sequenced<>>>;

    InstructionContainer instructions;
    ConstantContainer constants;
    MemberVarContainer memberVariables;
    ReferenceContainer references;
};
