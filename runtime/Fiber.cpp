
#include "runtime/Fiber.h"
#include <cstdint>
#include <cstdio>
#include "bytecode/Instruction.h"
#include "runtime/Frame.h"
#include "runtime/Value.h"
#include "runtime/handlers/binary.h"

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
                case bytecode::Opcode::Add: {
                    bytecode::Add instr = frame->read_at<bytecode::Add>(pc);
                    registers[instr.reg] =
                            Handlers::handle_add(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Sub: {
                    bytecode::Sub instr = frame->read_at<bytecode::Sub>(pc);
                    registers[instr.reg] =
                            Handlers::handle_sub(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Mul: {
                    bytecode::Mul instr = frame->read_at<bytecode::Mul>(pc);
                    registers[instr.reg] =
                            Handlers::handle_mul(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Div: {
                    bytecode::Div instr = frame->read_at<bytecode::Div>(pc);
                    registers[instr.reg] =
                            Handlers::handle_div(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
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
