cmake_minimum_required(VERSION 4.3)

project(nyx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(mimalloc REQUIRED)

set(PARSER_SRCS parser/Ast.cpp
        parser/Lexer.cpp
        parser/Parser.cpp)


set(IR_SRCS )

set(BYTECODE_SRCS bytecode/Executable.cpp
        bytecode/Generator.cpp
        bytecode/InstructionEmitter.cpp
        bytecode/RegisterAllocator.cpp)

set(HEAP_SRCS heap/Bin.cpp
        heap/Block.cpp
        heap/BlockAllocator.cpp
        heap/Heap.cpp
        heap/Region.cpp
        heap/RegionAllocator.cpp)

set(RUNTIME_SRCS runtime/CallStack.cpp runtime/Fiber.cpp runtime/FrameChunk.cpp)

set(SOURCES main.cpp ${PARSER_SRCS} ${BYTECODE_SRCS} ${HEAP_SRCS} ${RUNTIME_SRCS})

add_executable(nyx ${SOURCES})

target_include_directories(nyx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(nyx PRIVATE mimalloc)
