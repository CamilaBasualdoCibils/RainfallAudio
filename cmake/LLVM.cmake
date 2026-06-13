include(FetchContent)

# -------------------------------------------------
# LLVM configuration (MUST be before MakeAvailable)
# -------------------------------------------------
find_package(ZLIB REQUIRED)
set(LLVM_ENABLE_ZLIB ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    llvm
    URL https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-21.1.8.tar.gz
)

# Only build LLVM (no clang/lld/etc)
set(LLVM_ENABLE_PROJECTS "" CACHE STRING "" FORCE)
set(LLVM_ENABLE_RUNTIMES "" CACHE STRING "" FORCE)

# Targets
set(LLVM_TARGETS_TO_BUILD "X86" CACHE STRING "" FORCE)

# Disable optional deps
set(LLVM_ENABLE_TERMINFO OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_ZLIB OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_ZSTD OFF CACHE BOOL "" FORCE)

# Disable extras
set(LLVM_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "" FORCE)

# Static LLVM
set(LLVM_BUILD_LLVM_DYLIB OFF CACHE BOOL "" FORCE)
set(LLVM_LINK_LLVM_DYLIB OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# -------------------------------------------------
# Fetch & build LLVM
# -------------------------------------------------
FetchContent_MakeAvailable(llvm)

# -------------------------------------------------
# LLVM CMake helpers
# -------------------------------------------------
set(LLVM_DIR "${llvm_BINARY_DIR}/lib/cmake/llvm" CACHE PATH "" FORCE)
set(CMAKE_PREFIX_PATH "${llvm_BINARY_DIR}" CACHE PATH "" FORCE)

find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    IRReader
    ExecutionEngine
    MC
    MCJIT
    OrcJIT
    X86
)