add_llvm_library( LLVMADCEPass MODULE
        OurCFG.cpp
        ADCEPass.cpp

        PLUGIN_TOOL
        opt
)