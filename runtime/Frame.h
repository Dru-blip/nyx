#pragma once

#include <cstdint>
#include "bytecode/Executable.h"
#include "bytecode/Instruction.h"
#include "heap/Cell.h"


namespace Nyx {
    struct Frame {
        Frame(bytecode::Executable *executable) : m_executable(executable) {};

        uint8_t *get_code() { return m_executable->data(); }
        uint8_t *code_end() { return m_executable->data() + m_executable->size(); }

        template<typename Instr>
        Instr read_at(uint8_t *pc) {
            Instr instr;
            memcpy(&instr, pc, sizeof(Instr));
            return instr;
        }

        Frame *m_prev = nullptr;
        bytecode::Executable *m_executable;
        uint8_t *m_pc = nullptr;
    };
} // namespace Nyx
