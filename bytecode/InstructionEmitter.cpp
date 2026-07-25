
#include "InstructionEmitter.h"
#include "bytecode/Instruction.h"


namespace Nyx::bytecode {
    void InstructionEmitter::emit_ret_imm(int64_t imm) {
        RetImm ret_imm{Opcode::RetImm, imm};
        emit(ret_imm);
    }
} // namespace Nyx::bytecode
