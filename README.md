# LLVM Tail Call Elimination Pass

In-tree LLVM pass implementing tail call elimination — transforms tail-recursive
calls into loops, replacing recursion with a branch back to the function entry.

## Requirements

- LLVM 17
- Clang 17
- CMake 3.20+
- Make

## Build

Configure and build LLVM with the pass (from the repository root):

```
./make_llvm.sh
```

Or, if LLVM is already configured, build just the pass:

```
cd build && make LLVMTCEPass && cd ..
```

The plugin is produced at `build/lib/LLVMTCEPass.so`.

## Usage

Compile input to LLVM IR:

```
clang -O0 -Xclang -disable-O0-optnone -emit-llvm -S example_tce.c -o example_tce.ll
```

Run the pass (legacy pass manager):

```
opt -load build/lib/LLVMTCEPass.so --enable-new-pm=0 \
    -tce-pass \
    example_tce.ll -o example_tce_out.ll -S
```
