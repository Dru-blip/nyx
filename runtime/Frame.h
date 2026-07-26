#pragma once

#include <cstdint>
#include "bytecode/Executable.h"
#include "bytecode/Instruction.h"
#include "heap/Cell.h"


namespace Nyx {
    struct Frame {
        Frame(bytecode::Executable *executable) : m_executable(executable) {};

        Frame *prev = nullptr;
        bytecode::Executable *m_executable;

        uint32_t base;
        uint32_t m_pc;

        bytecode::Opcode get_opcode() {
            return reinterpret_cast<bytecode::Opcode &>(m_executable->data()[m_pc]);
        }

        template<typename Instr>
        Instr *decode_instruction() {
            Instr *instr = reinterpret_cast<Instr *>(m_executable->data() + m_pc);
            m_pc += sizeof(Instr);
            return instr;
        }
    };
} // namespace Nyx
