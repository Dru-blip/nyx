#include "ir/Instruction.h"
#include "ir/BasicBlock.h"

namespace Nyx::ir {
    void LoadImmInt::lower(InstructionEmitter &emitter) { emitter.load_imm_int(m_reg, m_value); }

    void Move::lower(InstructionEmitter &emitter) { emitter.move(m_src, m_dst); }

    void Not::lower(InstructionEmitter &emitter) { emitter.not_(m_value, m_dst); }

    void Neg::lower(InstructionEmitter &emitter) { emitter.neg(m_value, m_dst); }

    void Add::lower(InstructionEmitter &emitter) { emitter.add(m_lhs, m_rhs, m_dst); }

    void Sub::lower(InstructionEmitter &emitter) { emitter.sub(m_lhs, m_rhs, m_dst); }

    void Mul::lower(InstructionEmitter &emitter) { emitter.mul(m_lhs, m_rhs, m_dst); }

    void Div::lower(InstructionEmitter &emitter) { emitter.div(m_lhs, m_rhs, m_dst); }

    void Lt::lower(InstructionEmitter &emitter) { emitter.lt(m_lhs, m_rhs, m_dst); }

    void Lte::lower(InstructionEmitter &emitter) { emitter.lte(m_lhs, m_rhs, m_dst); }

    void Gt::lower(InstructionEmitter &emitter) { emitter.gt(m_lhs, m_rhs, m_dst); }

    void Gte::lower(InstructionEmitter &emitter) { emitter.gte(m_lhs, m_rhs, m_dst); }

    void Eq::lower(InstructionEmitter &emitter) { emitter.eq(m_lhs, m_rhs, m_dst); }

    void Neq::lower(InstructionEmitter &emitter) { emitter.neq(m_lhs, m_rhs, m_dst); }

    void Ret::lower(InstructionEmitter &emitter) { emitter.ret(m_value); }

    void JmpIfFalse::lower(InstructionEmitter &emitter) {
        emitter.jmp_if_false(m_condition, m_target->code_offset());
    }

    void JmpIfTrue::lower(InstructionEmitter &emitter) {
        emitter.jmp_if_true(m_condition, m_target->code_offset());
    }
} // namespace Nyx::ir
