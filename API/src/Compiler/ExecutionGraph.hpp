#pragma once
#include "Instruction/Instruction.hpp"
#include "Variable/Reference.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/detail/adjacency_list.hpp"
#include "boost/graph/graph_selectors.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index_container_fwd.hpp"
#include <taskflow/taskflow.hpp>
class ExecutionGraph;

class ExecutionGraph
{

  public:
    class InstructionIterator
    {
    };
    ExecutionGraph();
    template <typename T> InstructionIterator AddInstruction()
    {
        std::shared_ptr<T> ins = std::make_shared<T>();
        instructions.insert(ins);
    }

  private:
    // boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    //                       std::shared_ptr<InstructionInstance>,
    //                       std::shared_ptr<VariableReference>>
    //     graph;;

    struct InstructionsByType
    {
    };
    using InstructionContainer =
        boost::multi_index_container<std::shared_ptr<IInstruction>,
                                     boost::multi_index::indexed_by<boost::multi_index::sequenced<
                                         boost::multi_index::tag<InstructionsByType>
                                         // boost::multi_index::member<IInstruction,
                                         // std::type_index,&InstructionInstance::instruction_type>

                                         >>>;
    InstructionContainer instructions;

    using ReferenceContainer = boost::multi_index_container<
        std::shared_ptr<Reference>,
        boost::multi_index::indexed_by<boost::multi_index::sequenced<>>>;
};