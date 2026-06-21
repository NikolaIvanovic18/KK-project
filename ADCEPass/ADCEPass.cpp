//
// Created by andrija on 21. 6. 2026..
//

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"

#include "ADCEPass/OurCFG.h"
#include <set>
#include <queue>

#include "OurCFG.h"

using namespace llvm;

namespace {
    struct OurADCEPass: public FunctionPass {
        std::vector<Instruction*> InstructionsToRemove;
        bool InstructionRemoved;

        static char ID;
        OurADCEPass(): FunctionPass(ID) {}

        void elimanteDeadInstructions(Function& F) {
            InstructionsToRemove.clear();

            std::set<Instruction*> Alive;
            std::queue<Instruction*> Worklist;

            // Markiramo sve sto je zivo - terminator, store, call...
            // U ADCE nam je pocetna pretpostavka da je sve mrtvo
            for (BasicBlock& BB: F) {
                for (Instruction& I: BB) {
                    bool isAlive = false;

                    if (I.isTerminator()) {
                        isAlive = true;
                    }
                    else if (isa<StoreInst>(&I)) {
                        isAlive = true;
                    }
                    else if (isa<CallInst>(&I)) {
                        CallInst* Call = cast<CallInst>(&I);
                        if (!Call->doesNotAccessMemory()) {
                            isAlive = true;
                        }
                    }

                    if (isAlive) {
                        Alive.insert(&I);
                        Worklist.push(&I);
                    }
                }
            }

            // Propagiramo zivost unazad
            // Ako je instrukcija ziva, i njeni operandi moraju biti zivi
            while (!Worklist.empty()) {
                Instruction* I = Worklist.front();
                Worklist.pop();

                for (Use& U: I->operands()) {
                    if (Instruction* Op = dyn_cast<Instruction>(&U)) {
                        if (!Alive.count(Op)) {
                            Alive.insert(Op);
                            Worklist.push(Op);
                        }
                    }
                }
            }

            // Sve sto nije u Alive je mrtvo i to brisemo
            // replaceAllUsesWith(undef) koristimo jer LLVM ne dozvoljava brisanje
            // instrukcija koja ima korisnike, undef govori LLVM da se ona moze ignorisati
            // Terminatore nikad ne vrisemo, BB uvek mora ima tacno 1
            for (BasicBlock& BB: F) {
                for (Instruction& I: BB) {
                    if (!Alive.count(&I) && !I.isTerminator()) {
                        InstructionsToRemove.push_back(&I);
                    }
                }
            }

            if (!InstructionsToRemove.empty()) {
                InstructionRemoved = true;
            }

            for (Instruction* Instr: InstructionsToRemove) {
                if (!Instr->getType()->isVoidTy()) {
                    Instr->replaceAllUsesWith(UndefValue::get(Instr->getType()));
                }
                Instr->eraseFromParent();
            }
        }

        // Brisemo sve BB koji nisu dostizni, ovo dobijamo iz DFS
        void eliminateUnreachableInstructions(Function& F) {
            std::vector<BasicBlock*> UnreachableBlocks;

            OurCFG* CFG = new OurCFG(F);
            CFG->DFS(&F.front());

            for (BasicBlock& BB: F) {
                if (!CFG->isReachable(&BB)) {
                    UnreachableBlocks.push_back(&BB);
                }
            }

            for (BasicBlock* BB: UnreachableBlocks) {
                BB->eraseFromParent();
            }
        }

        bool runOnFunction(Function& F) override {
            do {
                InstructionsRemoved = false;
                elimanteDeadInstructions(F);
                elimanteDeadInstructions(F);
            } while (InstructionRemoved);

            return true;
        }
    };
}

char OurADCEPass::ID = 0;
static RegisterPass<OurADCEPass> X(
    "aggressive-dead-code-elimination",
    "Our Aggressive Dead Code Elimination Pass",
    false,
    false
    );