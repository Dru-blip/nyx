#pragma once

#include <cstdint>
#include "bytecode/Executable.h"
#include "heap/Cell.h"


namespace Nyx {
    struct Frame {
        Frame(bytecode::Executable *executable) : m_executable(executable) {};

        Frame *prev = nullptr;
        bytecode::Executable *m_executable;
        uint32_t base;
        uint32_t m_pc;
    };
} // namespace Nyx
