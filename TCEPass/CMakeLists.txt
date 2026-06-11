add_llvm_library( LLVMTCEPass MODULE
        TCEPass.cpp

        PLUGIN_TOOL
        opt
)