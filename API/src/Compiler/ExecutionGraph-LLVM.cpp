#include "ExecutionGraph.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
std::unique_ptr<llvm::Module>
ExecutionGraph::EmitLLVM(llvm::LLVMContext &Context)
{
    auto module =
        std::make_unique<llvm::Module>("ExecutionGraphModule", Context);
    llvm::IRBuilder<> Builder(Context);
    // main function type aka entry

    auto *FuncType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
    // entry function
    auto *MainFunc = llvm::Function::Create(
        FuncType, llvm::Function::ExternalLinkage, "main", module.get());
    // the block of the entry function
    auto *BB = llvm::BasicBlock::Create(Context, "entry", MainFunc);
    Builder.SetInsertPoint(BB);
   /*  /* 
    // --- Map names to LLVM Values ---
    std::map<std::string, llvm::Value *> NamedValues;
 */
     // --- Emit constants ---
    for (const auto &cons : constants)
    {
        
      //   auto *Alloca =
      //      Builder.CreateAlloca(cons->llvmType(Context), nullptr, "what");
      //  Builder.CreateStore(
      //      llvm::ConstantInt::get(Context, llvm::APInt(32, cons.Value)),
      //      Alloca);
      //  NamedValues[cons.Name] = Builder.CreateLoad(Alloca, cons.Name + "_val"); */
    } 

    // --- Emit instructions ---
    for (auto &ins : instructions)
    {
       /*  llvm::Value *Op1 = NamedValues[ins.Operand1];
        llvm::Value *Op2 = NamedValues[ins.Operand2];
        llvm::Value *Result = nullptr;

        if (ins.Op == "add")
        {
            Result = Builder.CreateAdd(Op1, Op2, ins.Name);
        }
        else if (ins.Op == "mul")
        {
            Result = Builder.CreateMul(Op1, Op2, ins.Name);
        }

        NamedValues[ins.Name] = Result; */
    }

    /* Builder.CreateRet(
        NamedValues[instructions.back().Name]); // return last instruction
                                                // result */

    // Verify module
    //if (llvm::verifyModule(*module, &llvm::errs()))
    //{
    //    llvm::errs() << "Module verification failed!\n";
    //}
 
    return module;
}