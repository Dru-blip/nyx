
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

    uint8_t InstructionEmitter::neg(const uint8_t &arg) {
        return emit_unary<Neg>(Opcode::Neg, arg);
    }

    uint8_t InstructionEmitter::not_(const uint8_t &arg) {
        return emit_unary<Not>(Opcode::Not, arg);
    }

    uint8_t InstructionEmitter::add(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Add>(Opcode::Add, left, right);
    }

    uint8_t InstructionEmitter::sub(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Sub>(Opcode::Sub, left, right);
    }


    uint8_t InstructionEmitter::mul(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Mul>(Opcode::Mul, left, right);
    }

    uint8_t InstructionEmitter::div(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Div>(Opcode::Div, left, right);
    }

    uint8_t InstructionEmitter::lt(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Lt>(Opcode::Lt, left, right);
    }

    uint8_t InstructionEmitter::lte(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Lte>(Opcode::Lte, left, right);
    }

    uint8_t InstructionEmitter::gt(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Gt>(Opcode::Gt, left, right);
    }

    uint8_t InstructionEmitter::gte(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Gte>(Opcode::Gte, left, right);
    }

    uint8_t InstructionEmitter::eq(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Eq>(Opcode::Eq, left, right);
    }

    uint8_t InstructionEmitter::neq(const uint8_t &left, const uint8_t &right) {
        return emit_binary<Neq>(Opcode::Neq, left, right);
    }

} // namespace Nyx::bytecode
