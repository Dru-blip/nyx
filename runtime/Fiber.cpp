
#include "runtime/Fiber.h"
#include "bytecode/Instruction.h"
#include "runtime/Frame.h"
#include "runtime/Value.h"

namespace Nyx {
    Value Fiber::run(bytecode::Executable *executable) {
        Frame *frame = m_stack.push(executable);
        size_t code_size = frame->m_executable->size();
        while (frame->m_pc < code_size) {
            bytecode::Opcode op = frame->get_opcode();
            switch (op) {
                case bytecode::Opcode::RetImm: {
                    bytecode::RetImm *instr = frame->decode_instruction<bytecode::RetImm>();
                    return Value::from_int(instr->value);
                }
                default: {
                    break;
                }
            }
        }

        return nil();
    }
} // namespace Nyx
