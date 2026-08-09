
#include "runtime/Fiber.h"
#include <cstdint>
#include "bytecode/Instruction.h"
#include "runtime/Frame.h"
#include "runtime/Value.h"

namespace Nyx {
    Value Fiber::run(bytecode::Executable *executable) {
        Frame *frame = m_stack.push(executable);

        uint8_t *pc = frame->get_code();
        frame->m_pc = pc;

        const uint8_t *code_end = frame->code_end();
        while (pc < code_end) {
            bytecode::Opcode op = static_cast<bytecode::Opcode>(*pc);
            switch (op) {
                case bytecode::Opcode::Ret: {
                    bytecode::Ret instr = frame->read_at<bytecode::Ret>(pc);
                    pc += sizeof(bytecode::Ret);
                    return Nil;
                }
                default: {
                    break;
                }
            }
        }
        return Nil;
    }
} // namespace Nyx
