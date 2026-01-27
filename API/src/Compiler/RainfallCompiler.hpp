#pragma once
#include "Instruction/Instruction.hpp"
#include "boost/graph/graph_selectors.hpp"
#include "misc/Singleton.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
class RainfallCompiler : public Singleton<RainfallCompiler>
{

    using InstructionDAG = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                                 std::shared_ptr<IInstruction>>;
    void SetInput(const InstructionDAG &DAG) {}
};