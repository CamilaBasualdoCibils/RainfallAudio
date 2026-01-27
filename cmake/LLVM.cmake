include(FetchContent)

# -----------------------------
# Fetch LLVM
# -----------------------------
FetchContent_Declare(
    llvm
    URL https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-21.1.8.tar.gz
    #GIT_TAG llvmorg-18.1.8   # pick a released tag, NOT main
)

# LLVM is extremely configurable — set options BEFORE MakeAvailable
set(LLVM_ENABLE_PROJECTS "" CACHE STRING "" FORCE)   # no clang/lld/etc
set(LLVM_ENABLE_RUNTIMES "" CACHE STRING "" FORCE)

set(LLVM_TARGETS_TO_BUILD "X86" CACHE STRING "" FORCE)
# or: "X86;ARM;AArch64" if you need more

set(LLVM_ENABLE_TERMINFO OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_ZLIB OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_ZSTD OFF CACHE BOOL "" FORCE)

set(LLVM_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "" FORCE)

set(LLVM_BUILD_LLVM_DYLIB OFF CACHE BOOL "" FORCE)
set(LLVM_LINK_LLVM_DYLIB OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(llvm)