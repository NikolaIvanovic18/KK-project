//
// Created by andrija on 21. 6. 2026..
//

#ifndef KK_PROJECT_OURCFG_H
#define KK_PROJECT_OURCFG_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Casting.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace llvm;

class OurCFG {
private:
    std::string FunctionName;
    std::unordered_set<BasicBlock *> Visited;
    std::unordered_map<BasicBlock *, std::vector<BasicBlock *>> AdjacencyList;
    void CreateCFG(Function &);
public:
    OurCFG(Function &);
    void DFS(BasicBlock *);
    bool isReachable(BasicBlock *);
};

#endif //KK_PROJECT_OURCFG_H
