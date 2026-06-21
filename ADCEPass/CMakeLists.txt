add_llvm_library( LLVMTCEPass MODULE
        OurCFG.cpp

        PLUGIN_TOOL
        opt
)