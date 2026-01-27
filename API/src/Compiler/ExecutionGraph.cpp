#include "ExecutionGraph.hpp"
ExecutionGraph::value_iterator ExecutionGraph::internal_GetMemberVar(InstructionID ins_id,
                                                                     const std::string_view str)
{
    const auto &view = memberVariables.get<ByInstructionID>();
    const auto range = view.equal_range(ins_id);
    for (auto it = range.first; it != range.second; it++)
    {
        if ((*it)->Name == str)
            return {*this, *it};
    }
    return {*this, nullptr};
}
ExecutionGraph::value_iterator ExecutionGraph::internal_GetMemberVar(InstructionID ins_id,
                                                                     ScopeVariableID var_id)
{
    const auto &view = memberVariables.get<ByInstructionID>();
    const auto range = view.equal_range(ins_id);
    for (auto it = range.first; it != range.second; it++)
    {
        if ((*it)->ID == var_id)
            return {*this, *it};
    }
    return {*this, nullptr};
}
