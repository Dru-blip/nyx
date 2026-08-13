#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include "bytecode/Instruction.h"
#include "bytecode/JmpPatch.h"
#include "bytecode/RegisterAllocator.h"


namespace Nyx::bytecode {
    class InstructionEmitter {
    public:
        template<typename InstType>
        void emit(const InstType &inst) {
            m_code.resize(m_code.size() + sizeof(InstType));
            std::memcpy(m_code.data() + m_code.size() - sizeof(InstType), &inst, sizeof(InstType));
        }


        template<typename Instr>
        uint8_t emit_binary(Opcode opcode, const uint8_t &arg1, const uint8_t &arg2) {
            m_register_allocator.free(arg1);
            m_register_allocator.free(arg2);

            uint8_t reg = m_register_allocator.allocate();

            push(static_cast<uint8_t>(opcode));

            Instr inst{arg1, arg2, reg};
            emit<Instr>(inst);

            return reg;
        }


        template<typename Instr>
        uint8_t emit_unary(Opcode opcode, const uint8_t &arg) {
            push(static_cast<uint8_t>(opcode));
            m_register_allocator.free(arg);
            uint8_t result = m_register_allocator.allocate();
            Instr inst{arg, result};
            emit<Instr>(inst);
            return result;
        }


        template<typename Instr>
        void patch(JmpPatch &patch) {
            patch.patch<Instr>(m_code, m_code.size());
        }

        void push(const uint8_t byte) { m_code.push_back(byte); }

        std::vector<uint8_t> &code() { return m_code; }
        void ret(const uint8_t reg);
        uint8_t load_imm_int(const int64_t imm);

        uint8_t move(const uint8_t& src, const uint8_t &dst);

        uint8_t not_(const uint8_t &arg);
        uint8_t neg(const uint8_t &arg);
        uint8_t add(const uint8_t &left, const uint8_t &right);
        uint8_t sub(const uint8_t &left, const uint8_t &right);
        uint8_t mul(const uint8_t &left, const uint8_t &right);
        uint8_t div(const uint8_t &left, const uint8_t &right);

        uint8_t lt(const uint8_t &left, const uint8_t &right);
        uint8_t lte(const uint8_t &left, const uint8_t &right);
        uint8_t gt(const uint8_t &left, const uint8_t &right);
        uint8_t gte(const uint8_t &left, const uint8_t &right);
        uint8_t eq(const uint8_t &left, const uint8_t &right);
        uint8_t neq(const uint8_t &left, const uint8_t &right);

        JmpPatch emit_jmpif_false_patch(const uint8_t &cond);
        JmpPatch emit_jmpif_false_move_patch(const uint8_t &cond);

    private:
        std::vector<uint8_t> m_code;
        RegisterAllocator m_register_allocator;
    };
} // namespace Nyx::bytecode
