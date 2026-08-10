
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
        Value *registers = frame->get_registers();

        while (pc < code_end) {
            bytecode::Opcode op = static_cast<bytecode::Opcode>(*pc++);
            switch (op) {
                case bytecode::Opcode::LoadImmInt: {
                    bytecode::LoadImmInt instr = frame->read_at<bytecode::LoadImmInt>(pc);
                    registers[instr.reg] = Value(instr.imm);
                    break;
                }
                // case bytecode::Opcode::Add: {
                //     bytecode::Add instr = frame->read_at<bytecode::Add>(pc);
                //     pc += sizeof(bytecode::Add);
                //     registers[instr.reg] = registers[instr.lhs] + registers[instr.rhs];
                //     break;
                // }
                case bytecode::Opcode::Ret: {
                    bytecode::Ret instr = frame->read_at<bytecode::Ret>(pc);
                    return registers[instr.reg];
                }
                default: {
                    break;
                }
            }
        }
        return Nil;
    }
} // namespace Nyx
