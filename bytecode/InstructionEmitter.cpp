
#include "InstructionEmitter.h"
#include <cstdint>
#include "bytecode/Instruction.h"


namespace Nyx::bytecode {
    void InstructionEmitter::ret() { emit<Ret>(Opcode::Ret); }

    void InstructionEmitter::load_imm_int(const int64_t &imm) {
        emit<LoadImmInt>(Opcode::LoadImmInt, imm);
    }

    void InstructionEmitter::load_const(const uint16_t &idx) {
        emit<LoadConst>(Opcode::LoadConst, idx);
    }

    void InstructionEmitter::load_string(const uint16_t &idx) {
        emit<LoadString>(Opcode::LoadString, idx);
    }

    void InstructionEmitter::store_local(const uint8_t &slot) {
        emit<StoreLocal>(Opcode::StoreLocal, slot);
    }


    void InstructionEmitter::neg() { emit<Neg>(Opcode::Neg); }

    void InstructionEmitter::not_() { emit<Not>(Opcode::Not); }

    void InstructionEmitter::add() { emit<Add>(Opcode::Add); }

    void InstructionEmitter::sub() { emit<Sub>(Opcode::Sub); }

    void InstructionEmitter::mul() { emit<Mul>(Opcode::Mul); }

    void InstructionEmitter::div() { emit<Div>(Opcode::Div); }

    void InstructionEmitter::lt() { emit<Lt>(Opcode::Lt); }

    void InstructionEmitter::lte() { emit<Lte>(Opcode::Lte); }

    void InstructionEmitter::gt() { emit<Gt>(Opcode::Gt); }

    void InstructionEmitter::gte() { emit<Gte>(Opcode::Gte); }

    void InstructionEmitter::eq() { emit<Eq>(Opcode::Eq); }

    void InstructionEmitter::neq() { emit<Neq>(Opcode::Neq); }

    void InstructionEmitter::jmp(const uint16_t &offset) { emit<Jmp>(Opcode::Jmp, offset); }

    void InstructionEmitter::jmp_if_false(const uint32_t &offset) {
        emit<JmpIfFalse>(Opcode::JmpIfFalse, offset);
    }

    void InstructionEmitter::jmp_if_true(const uint32_t &offset) {
        emit<JmpIfTrue>(Opcode::JmpIfTrue, offset);
    }

    void InstructionEmitter::ret_nil() { push(static_cast<uint8_t>(Opcode::RetNil)); }

    void InstructionEmitter::call(const uint8_t &arg_count) { emit<Call>(Opcode::Call, arg_count); }

} // namespace Nyx::bytecode
