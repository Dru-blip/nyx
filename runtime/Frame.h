#pragma once

#include <bit>
#include <cstdint>
#include <cstring>
#include "bytecode/Executable.h"
#include "bytecode/Instruction.h"
#include "heap/Cell.h"
#include "runtime/Value.h"


namespace Nyx {
    struct Frame {
        Frame(bytecode::Executable *executable) : m_executable(executable) {};

        inline uint8_t *get_code() { return m_executable->data(); }
        inline uint8_t *code_end() { return m_executable->data() + m_executable->size(); }

        template<typename Instr>
        inline Instr read_at(uint8_t *&pc) {
            Instr instr;
            std::memcpy(&instr, pc, sizeof(Instr));
            pc += sizeof(Instr);
            return instr;
        }

        inline Value *locals() { return reinterpret_cast<Value *>(this + 1); }
        inline Value *stack() { return locals() + m_executable->num_locals(); }
        inline Value *constants() { return m_executable->constants(); }

        inline size_t size() {
            return sizeof(Frame) +
                   (m_executable->stack_size() + m_executable->num_locals()) * sizeof(Value);
        }


        Frame *m_prev = nullptr;
        bytecode::Executable *m_executable;
        uint8_t *m_pc = nullptr;
        uint8_t m_tos = 0;
    };
} // namespace Nyx
