
#include "InstructionEmitter.h"
#include <cstdint>
#include "bytecode/Instruction.h"
#include "ir/Register.h"


namespace Nyx::bytecode {
    void InstructionEmitter::ret(const Register &reg) { emit<Ret>(Opcode::Ret, reg.slot()); }

    void InstructionEmitter::load_imm_int(const Register &reg, const int64_t &imm) {
        emit<LoadImmInt>(Opcode::LoadImmInt, reg.slot(), imm);
    }

    void InstructionEmitter::move(const Register &src, const Register &dst) {
        emit<Move>(Opcode::Move, src.slot(), dst.slot());
    }

    void InstructionEmitter::neg(const Register &arg, const Register &dst) {
        emit<Neg>(Opcode::Neg, arg.slot(), dst.slot());
    }

    void InstructionEmitter::not_(const Register &arg, const Register &dst) {
        emit<Not>(Opcode::Not, arg.slot(), dst.slot());
    }

    void InstructionEmitter::add(const Register &left, const Register &right, const Register &dst) {
        emit<Add>(Opcode::Add, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::sub(const Register &left, const Register &right, const Register &dst) {
        emit<Sub>(Opcode::Sub, left.slot(), right.slot(), dst.slot());
    }


    void InstructionEmitter::mul(const Register &left, const Register &right, const Register &dst) {
        emit<Mul>(Opcode::Mul, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::div(const Register &left, const Register &right, const Register &dst) {
        emit<Div>(Opcode::Div, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::lt(const Register &left, const Register &right, const Register &dst) {
        emit<Lt>(Opcode::Lt, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::lte(const Register &left, const Register &right, const Register &dst) {
        emit<Lte>(Opcode::Lte, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::gt(const Register &left, const Register &right, const Register &dst) {
        emit<Gt>(Opcode::Gt, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::gte(const Register &left, const Register &right, const Register &dst) {
        emit<Gte>(Opcode::Gte, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::eq(const Register &left, const Register &right, const Register &dst) {
        emit<Eq>(Opcode::Eq, left.slot(), right.slot(), dst.slot());
    }

    void InstructionEmitter::neq(const Register &left, const Register &right, const Register &dst) {
        emit<Neq>(Opcode::Neq, left.slot(), right.slot(), dst.slot());
    }

} // namespace Nyx::bytecode
