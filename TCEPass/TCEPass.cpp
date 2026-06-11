#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct TCEPass : public FunctionPass {
    static char ID;
    TCEPass() : FunctionPass(ID) {}

    static bool isTailCall(CallInst *CI) {
        Instruction *next = CI->getNextNode();

        if (auto *RI = dyn_cast<ReturnInst>(next)) {
            if (CI->getType()->isVoidTy())
                return RI->getReturnValue() == nullptr;
            return RI->getReturnValue() == CI;
        }

        if (auto *BI = dyn_cast<BranchInst>(next)) {
            if (!CI->getType()->isVoidTy() || !BI->isUnconditional())
                return false;
            auto *RI = dyn_cast<ReturnInst>(&BI->getSuccessor(0)->front());
            return RI && RI->getReturnValue() == nullptr;
        }

        if (auto *SI = dyn_cast<StoreInst>(next))
            return SI->getValueOperand() == CI;

        return false;
    }

    bool runOnFunction(Function &F) override {
        SmallVector<CallInst*, 8> tailCalls;

        for (auto &BB : F)
            for (auto &I : BB)
                if (auto *CI = dyn_cast<CallInst>(&I))
                    if (CI->getCalledFunction() == &F && isTailCall(CI))
                        tailCalls.push_back(CI);

        if (tailCalls.empty())
            return false;

        SmallVector<AllocaInst*, 8> paramAllocas(F.arg_size(), nullptr);
        for (auto &I : F.getEntryBlock()) {
            if (auto *SI = dyn_cast<StoreInst>(&I))
                if (auto *Arg = dyn_cast<Argument>(SI->getValueOperand()))
                    if (Arg->getParent() == &F)
                        if (auto *AI = dyn_cast<AllocaInst>(SI->getPointerOperand()))
                            paramAllocas[Arg->getArgNo()] = AI;
        }

        for (AllocaInst *AI : paramAllocas) {
            if (!AI) {
                errs() << "[TCE] Nije moguce transformisati " << F.getName()
                       << ": ne poklapaju se parametri\n";
                return false;
            }
        }

        BasicBlock *entryBB = &F.getEntryBlock();
        Instruction *splitPoint = nullptr;
        for (auto &I : *entryBB) {
            if (!isa<AllocaInst>(&I) && !isa<StoreInst>(&I)) {
                splitPoint = &I;
                break;
            }
        }
        if (!splitPoint)
            return false;

        BasicBlock *loopHeader = entryBB->splitBasicBlock(splitPoint, "loop.header");

        for (CallInst *CI : tailCalls)
            transformTailCall(CI, paramAllocas, loopHeader);

        return true;
    }

    static void transformTailCall(CallInst *CI, ArrayRef<AllocaInst*> paramAllocas,
                                  BasicBlock *loopHeader) {
        BasicBlock *callBB = CI->getParent();

        errs() << "[TCE] Transformisem tail call u: "
               << CI->getFunction()->getName() << "\n";

        IRBuilder<> builder(CI);
        for (unsigned i = 0; i < CI->arg_size(); i++)
            builder.CreateStore(CI->getArgOperand(i), paramAllocas[i]);

        if (auto *SI = dyn_cast<StoreInst>(CI->getNextNode()))
            SI->eraseFromParent();

        callBB->getTerminator()->eraseFromParent();
        builder.SetInsertPoint(callBB);
        builder.CreateBr(loopHeader);

        CI->eraseFromParent();
    }
};

} // namespace

char TCEPass::ID = 0;

static RegisterPass<TCEPass> X(
    "tce-pass",
    "Tail Call Elimination Pass",
    false,
    false
);
