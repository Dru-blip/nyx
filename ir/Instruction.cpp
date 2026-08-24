#include "ir/Instruction.h"
#include "ir/BasicBlock.h"

namespace Nyx::ir {
    void LoadImmInt::lower(InstructionEmitter &emitter) { emitter.load_imm_int(m_value); }

    void LoadConst::lower(InstructionEmitter &emitter) { emitter.load_const(m_idx); }

    void LoadString::lower(InstructionEmitter &emitter) { emitter.load_string(m_idx); }

    void Not::lower(InstructionEmitter &emitter) { emitter.not_(); }

    void Neg::lower(InstructionEmitter &emitter) { emitter.neg(); }

    void Add::lower(InstructionEmitter &emitter) { emitter.add(); }

    void Sub::lower(InstructionEmitter &emitter) { emitter.sub(); }

    void Mul::lower(InstructionEmitter &emitter) { emitter.mul(); }

    void Div::lower(InstructionEmitter &emitter) { emitter.div(); }

    void Lt::lower(InstructionEmitter &emitter) { emitter.lt(); }

    void Lte::lower(InstructionEmitter &emitter) { emitter.lte(); }

    void Gt::lower(InstructionEmitter &emitter) { emitter.gt(); }

    void Gte::lower(InstructionEmitter &emitter) { emitter.gte(); }

    void Eq::lower(InstructionEmitter &emitter) { emitter.eq(); }

    void Neq::lower(InstructionEmitter &emitter) { emitter.neq(); }

    void Ret::lower(InstructionEmitter &emitter) { emitter.ret(); }

    void JmpIfFalse::lower(InstructionEmitter &emitter) {
        // TODO: should convert from absolute offset to relative offset.
        emitter.jmp_if_false(m_target->code_offset());
    }

    void JmpIfTrue::lower(InstructionEmitter &emitter) {
        // TODO: should convert from absolute offset to relative offset.
        emitter.jmp_if_true(m_target->code_offset());
    }

    void Jmp::lower(InstructionEmitter &emitter) {
        // TODO: should convert from absolute offset to relative offset.
        emitter.jmp(m_target->code_offset());
    }

    void Branch::lower(InstructionEmitter &emitter) {
        emitter.jmp_if_true(m_true_target->code_offset());
        emitter.jmp(m_false_target->code_offset());
    }

    void RetNil::lower(InstructionEmitter &emitter) { emitter.ret_nil(); }

    void Call::lower(InstructionEmitter &emitter) { emitter.call(m_arg_count); }
} // namespace Nyx::ir
