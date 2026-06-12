# LLVM TCE & ADCE Pass

Out-of-tree LLVM passes implementing tail call elimination and aggressive dead
code elimination, designed to run ADCE after TCE.

## Requirements

- LLVM 17
- Clang 17
- CMake 3.20+
- Make

## Build

```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build
cd build && make && cd ..
```

## Usage

Compile input to LLVM IR:

```
clang -O0 -Xclang -disable-O0-optnone -emit-llvm -S example.c -o example.ll
```

Run passes:

```
opt -load build/libLLVMTCEPass.so \
    -load build/libLLVMADCEPass.so \
    --enable-new-pm=0 \
    -tce-pass -adce-pass \
    example.ll -o example_out.ll -S
```
