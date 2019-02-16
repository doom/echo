add_executable(echogen
        ${CMAKE_CURRENT_SOURCE_DIR}/src/annotated_match_callback.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        )

set(LLVM_PATH /usr/lib/llvm-7)

target_include_directories(echogen PRIVATE ${LLVM_PATH}/include)
target_link_directories(echogen PRIVATE ${LLVM_PATH}/lib)

target_link_libraries(echogen
        PRIVATE
        stdc++fs
        LLVM-7
        clangTooling
        clangFrontendTool
        clangFrontend
        clangDriver
        clangSerialization
        clangParse
        clangSema
        clangAnalysis
        clangEdit
        clangAST
        clangASTMatchers
        clangLex
        clangBasic
        clang
        )
