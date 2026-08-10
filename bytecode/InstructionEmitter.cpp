
#include "InstructionEmitter.h"
#include <cstdint>
#include "bytecode/Instruction.h"


namespace Nyx::bytecode {
    void InstructionEmitter::ret(const uint8_t reg) {
        Ret inst{reg};
        push(static_cast<uint8_t>(Opcode::Ret));
        emit(inst);
    }

    uint8_t InstructionEmitter::load_imm_int(const int64_t imm) {
        push(static_cast<uint8_t>(Opcode::LoadImmInt));
        uint8_t reg = m_register_allocator.allocate();
        LoadImmInt inst{reg, imm};
        emit(inst);
        return reg;
    }

    uint8_t InstructionEmitter::add(const uint8_t &left, const uint8_t &right) {
        m_register_allocator.free(right);
        m_register_allocator.free(left);

        uint8_t reg = m_register_allocator.allocate();

        push(static_cast<uint8_t>(Opcode::Add));

        Add inst{left, right, reg};
        emit(inst);

        return reg;
    }

    uint8_t InstructionEmitter::sub(const uint8_t &left, const uint8_t &right) {
        m_register_allocator.free(right);
        m_register_allocator.free(left);

        uint8_t reg = m_register_allocator.allocate();

        push(static_cast<uint8_t>(Opcode::Sub));
        Sub inst{left, right, reg};
        emit(inst);

        return reg;
    }


    uint8_t InstructionEmitter::mul(const uint8_t &left, const uint8_t &right) {
        m_register_allocator.free(right);
        m_register_allocator.free(left);

        uint8_t reg = m_register_allocator.allocate();

        push(static_cast<uint8_t>(Opcode::Mul));
        Mul inst{left, right, reg};
        emit(inst);

        return reg;
    }

    uint8_t InstructionEmitter::div(const uint8_t &left, const uint8_t &right) {
        m_register_allocator.free(right);
        m_register_allocator.free(left);

        uint8_t reg = m_register_allocator.allocate();

        push(static_cast<uint8_t>(Opcode::Div));
        Div inst{left, right, reg};
        emit(inst);

        return reg;
    }


} // namespace Nyx::bytecode
