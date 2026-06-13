#include "ExecutionGraph.hpp"
#include "Compiler/Instruction/Defines.hpp"
#include "boost/multi_index/composite_key.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/indexed_by.hpp"
#include "boost/multi_index/member.hpp"
#include <iostream>
#include <sstream>

ExecutionGraph::value_iterator
ExecutionGraph::internal_GetMemberVar(InstructionID ins_id,
                                      const std::string_view str)
{
    const auto &view = OutputMemberVariables.get<ByInstructionID>();
    const auto range = view.equal_range(ins_id);
    for (auto it = range.first; it != range.second; it++)
    {
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

    assert(instructions.get<ByInstructionID>().contains(to_instruction_id) &&
           "Invalid Instruction ID");
    assert((*instructions.get<ByInstructionID>().find(to_instruction_id))
               ->hasInputWithHash(to_instruction_member_var_hash) &&
           "Invalid Member Variable Hash");
    IReference ref;
    ref.ToInstructionID = to_instruction_id;
    ref.ToInstructionMemberHash = to_instruction_member_var_hash;
    ref.FromVarGlobalID = from_global_var_id;
    references.insert(ref);

    // const auto &instructonView = memberVariables.get<ByInstructionID>();
    // const auto range = instructionView.equal_range(instruction_id);
    // for (const auto &)
}
std::string ExecutionGraph::EmitDot()
{

    std::ostringstream dot;

    dot << "digraph ExecutionGraph {\n";
    dot << "  rankdir=UD;\n";
    // dot << "  node [fontname=\"monospace\"];\n\n";

    // --- constants nodes ---
    dot << "  // constants\n";
    for (const auto &cons : constants)
    {
        dot << "  var_" << cons->GlobalID << " " << cons->GetValue().EmitDOT()
            << "\n";
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

    struct InstructionMapping
    {
        InstructionID fromIns, toIns;
        GlobalVariableID FromGlobalID;
        MemberVarHash toMemberHash;
    };
    // From InstructionID/ From member hash
    using MappingContainer = boost::multi_index_container<
        InstructionMapping,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_non_unique<
                boost::multi_index::member<InstructionMapping, InstructionID,
                                           &InstructionMapping::fromIns>>,
            boost::multi_index::hashed_non_unique<
                boost::multi_index::member<InstructionMapping, GlobalVariableID,
                                           &InstructionMapping::FromGlobalID>>,
            boost::multi_index::hashed_non_unique<
                boost::multi_index::composite_key<
                    InstructionMapping,
                    boost::multi_index::member<InstructionMapping,
                                               InstructionID,
                                               &InstructionMapping::fromIns>,
                    boost::multi_index::member<
                        InstructionMapping, GlobalVariableID,
                        &InstructionMapping::FromGlobalID>>>>>;
    MappingContainer mappings;
    {

        for (const IReference &reference : references)
        {
            const GlobalVariableID fromGlobalID = reference.FromVarGlobalID;
            const bool isVarConstant =
                constants.get<ByGlobalID>().contains(fromGlobalID);
            const auto &MemberByGlobalIDView =
                OutputMemberVariables.get<ByGlobalID>();
            if (!isVarConstant)
            {
                InstructionMapping map;
                const InstructionID fromInstructionID =
                    MemberByGlobalIDView.find(fromGlobalID)->get()->CreatorID;
                map.fromIns = fromInstructionID;
                map.FromGlobalID = fromGlobalID;
                map.toIns = reference.ToInstructionID;
                map.toMemberHash = reference.ToInstructionMemberHash;
                mappings.insert(map);
            }
            else
            {
                const auto &outvarname =
                    instructions.get<ByInstructionID>()
                        .find(reference.ToInstructionID)
                        ->get()
                        ->GetInputFromHash(reference.ToInstructionMemberHash)
                        .Name;
                dot << "  var_" << reference.FromVarGlobalID << "->" << "inst_"
                    << reference.ToInstructionID
                    << std::format(" [style=dashed,shape=point, "
                                   "width=0.05,label=\"{}\"];\n",
                                   outvarname);
            }
        }

        const auto &fromInstructionView = mappings.get<2>();

        for (auto it = fromInstructionView.begin();
             it != fromInstructionView.end();)
        {
            auto range = fromInstructionView.equal_range(
                boost::make_tuple(it->fromIns, it->FromGlobalID));

            InstructionID fromIns = it->fromIns;
            GlobalVariableID globalID = it->FromGlobalID;
            const size_t refCount = std::distance(range.first, range.second);
            const std::string VarName = OutputMemberVariables.get<ByGlobalID>()
                                            .find(globalID)
                                            ->get()
                                            ->Name;

            {
                // unique junction per (fromIns, globalID)
                dot << "  junction_" << fromIns << "_" << globalID
                    << std::format(" [shape=point, width=0.05];\n");

                // from -> junction
                dot << "  inst_" << fromIns << " -> junction_" << fromIns << "_"
                    << globalID
                    << std::format(
                           " [style=dashed,label=\"{}\",arrowhead=none];\n",
                           VarName);

                // junction -> all targets
                const auto &instructionsByID =
                    instructions.get<ByInstructionID>();
                for (auto it2 = range.first; it2 != range.second; ++it2)
                {
                    const auto &outvarname =
                        instructionsByID.find(it2->toIns)
                            ->get()
                            ->GetInputFromHash(it2->toMemberHash)
                            .Name;
                    dot << "  junction_" << fromIns << "_" << globalID
                        << " -> inst_" << it2->toIns
                        << std::format(" [style=dashed,label=\"{}\"];\n",
                                       outvarname);
                }
            }

            it = range.second; // skip to next unique group
        }
    }

    dot << "}\n";
    return dot.str();
}
