#pragma once
#include "Instruction/Instruction.hpp"
#include "boost/graph/graph_selectors.hpp"
#include "Singleton.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
class RainfallCompiler : public Singleton<RainfallCompiler>
{

    public:
    void SetInput(const ExecutionGraph &DAG) {}
    bool Compile();
};