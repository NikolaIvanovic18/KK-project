//
// Created by andrija on 21. 6. 2026..
//

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"

#include "OurCFG.h"
#include <set>
#include <queue>

using namespace llvm;

namespace {
    struct OurADCEPass: public FunctionPass {
        std::vector<Instruction*> InstructionsToRemove;
        bool InstructionRemoved;

        static char ID;
        OurADCEPass(): FunctionPass(ID) {}

        void eliminateDeadInstructions(Function& F) {
            InstructionsToRemove.clear();

            std::set<Instruction*> Alive;
            std::queue<Instruction*> Worklist;

            // Markiramo sve sto je zivo
            // U ADCE nam je pocetna pretpostavka da je sve mrtvo
            for (BasicBlock& BB: F) {
                for (Instruction& I: BB) {
                    bool isAlive = false;

                    // Terminator (ret, br, switch) je uvek ziv
                    if (I.isTerminator()) {
                        isAlive = true;
                    }
                    // Store radi sa memorijom pa mora ostati
                    else if (isa<StoreInst>(&I)) {
                        isAlive = true;
                    }
                    else if (isa<CallInst>(&I)) {
                        CallInst* Call = cast<CallInst>(&I);
                        // Pristupa memoriji pa verovatno ima propratne efekte, pa ostaje
                        if (!Call->doesNotAccessMemory()) {
                            isAlive = true;
                        }
                    }

                    // Instrukcija je ziva
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

                // Uzimamo operande
                for (Use& U: I->operands()) {
                    if (Instruction* Op = dyn_cast<Instruction>(&U)) {
                        // Ako operand nije vec oznacen kao ziv ubacujemo ga u skup i red
                        // Posto je skup count vraca 1 ili 0
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
                    // Ako nije ziva ili nije terminator brisemo
                    if (!Alive.count(&I) && !I.isTerminator()) {
                        InstructionsToRemove.push_back(&I);
                    }
                }
            }

            // Obrisali smo nesto, ovo nam sluzi da se do-while opet izvrso
            if (!InstructionsToRemove.empty()) {
                InstructionRemoved = true;
            }

            for (Instruction* Instr: InstructionsToRemove) {
                // Ako povratna vrednost instrukcije nije void svako korisnike menjamo sa undef
                // Ovo radimo jer LLVM zahteva da instrukcija nema korisnike pre brisanja
                if (!Instr->getType()->isVoidTy()) {
                    Instr->replaceAllUsesWith(UndefValue::get(Instr->getType()));
                }
                // Uklanja se instrukcija iz BB i oslobadja se memorija
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

        // Brisemo sve dok se bar 1 instrukcija obrisala
        // Brisanje moze osloboditi neku novu instrukciju za brisanje
        bool runOnFunction(Function& F) override {
            do {
                InstructionRemoved = false;
                eliminateDeadInstructions(F);
                eliminateUnreachableInstructions(F);
            } while (InstructionRemoved);

            return true;
        }
    };
}

char OurADCEPass::ID = 0;
static RegisterPass<OurADCEPass> X(
    "adce",
    "Our Aggressive Dead Code Elimination Pass",
    false,
    false
    );