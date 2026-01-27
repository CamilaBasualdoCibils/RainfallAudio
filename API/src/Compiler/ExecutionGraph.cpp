#include "ExecutionGraph.hpp"
#include "Compiler/Instruction/Defines.hpp"
#include <sstream>
ExecutionGraph::value_iterator
ExecutionGraph::internal_GetMemberVar(InstructionID ins_id,
                                      const std::string_view str)
{
    const auto &view = OutputMemberVariables.get<ByInstructionID>();
    const auto range = view.equal_range(ins_id);
    for (auto it = range.first; it != range.second; it++)
    {
        std::cout << (*it)->Name << std::endl;
        if ((*it)->Name == str)
            return {*this, *it};
    }
    return {*this, nullptr};
}
ExecutionGraph::value_iterator
ExecutionGraph::internal_GetMemberVar(InstructionID ins_id,
                                      MemberVarHash var_hash)
{
    const auto &view =
        OutputMemberVariables.get<ByInstruction_MemberHash_Pair>();
    if (auto f = view.find(std::make_tuple(ins_id, var_hash)); f != view.end())
    {
        return {*this, *f};
    }

    return {*this, nullptr};
}
void ExecutionGraph::internal_CreateReference(
    GlobalVariableID from_global_var_id, InstructionID to_instruction_id,
    MemberVarHash to_instruction_member_var_hash)
{
    assert(
        (OutputMemberVariables.get<ByGlobalID>().contains(from_global_var_id) ||
         constants.get<ByGlobalID>().contains(from_global_var_id)) &&
        "No Global variable by that ID");
    for (const auto &v : OutputMemberVariables)
    {
        std::cout << v->CreatorID << " " << v->memberHash << std::endl;
    }

    assert(instructions.get<ByInstructionID>().contains(to_instruction_id) &&
           "Invalid Instruction ID");
    assert((*instructions.get<ByInstructionID>().find(to_instruction_id))
               ->hasInputWithHash(to_instruction_member_var_hash) &&
           "Invalid Member Variable Hash");
    IReference ref;
    ref.ToInstructionID = to_instruction_id;
    ref.ToInstructionMemberVarID = to_instruction_member_var_hash;
    ref.FromVarGlobalID = from_global_var_id;
    references.push_back(ref);
    // const auto &instructonView = memberVariables.get<ByInstructionID>();
    // const auto range = instructionView.equal_range(instruction_id);
    // for (const auto &)
}
std::string ExecutionGraph::EmitDot()
{

    std::ostringstream dot;

    dot << "digraph ExecutionGraph {\n";
    dot << "  rankdir=LR;\n";
    dot << "  node [fontname=\"monospace\"];\n\n";

    // --- constants nodes ---
    dot << "  // constants\n";
    for (const auto &cons : constants)
    {
        dot << "  var_" << cons->GlobalID << " [label=\"" << cons->Name
            << "\", shape=ellipse];\n";
    }

    dot << "\n";
    // --- variables nodes ---
    dot << "  // variables\n";
    for (const auto &var : OutputMemberVariables)
    {
        dot << "  var_" << var->GlobalID << " [label=\"" << var->Name
            << "\", shape=ellipse];\n";
            dot << "  inst_" << var->CreatorID << " -> var_"
            << var->GlobalID << ";\n";
    }

    dot << "\n";

    // --- Instruction nodes ---
    dot << "  // Instructions\n";
    for (const auto &ins : instructions)
    {
        dot << "  inst_" << ins->GetID() << " [label=\"" << ins->GetLabel()
            << "\", shape=box];\n";
    }

    dot << "\n";

    // --- Edges: variable -> instruction ---
    dot << "  // Uses\n";
    for (const auto &reference : references)
    {
        dot << "  var_" << reference.FromVarGlobalID << " -> inst_"
            << reference.ToInstructionID << ";\n";
    }

    dot << "}\n";

    return dot.str();
}
