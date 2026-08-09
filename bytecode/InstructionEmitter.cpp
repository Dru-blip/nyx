
#include "InstructionEmitter.h"
#include <cstdint>
#include "bytecode/Instruction.h"


namespace Nyx::bytecode {
    void InstructionEmitter::emit_ret_imm(const int64_t imm) {
        RetImm inst{Opcode::RetImm, imm};
        emit(inst);
    }

    void InstructionEmitter::emit_ret() { push(static_cast<uint8_t>(Opcode::Ret)); }

    uint8_t InstructionEmitter::emit_add(const Operand &left, const Operand &right) {
        if (left.isRegister() && right.isRegister()) {
            m_register_allocator.free(right.as.reg);
            m_register_allocator.free(left.as.reg);

            uint8_t reg = m_register_allocator.allocate();

            push(static_cast<uint8_t>(Opcode::Add));
            push(left.as.reg);
            push(right.as.reg);
            push(reg);

            return reg;
        }
    }
} // namespace Nyx::bytecode
