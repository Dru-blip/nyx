
#include "runtime/Fiber.h"
#include <cstdint>
#include <cstdio>
#include <print>
#include "bytecode/Instruction.h"
#include "runtime/Frame.h"
#include "runtime/Value.h"
#include "runtime/handlers/binary.h"

namespace Nyx {
    Value Fiber::run(bytecode::Executable *executable) {
        Frame *frame = m_stack.push(executable);

        uint8_t *pc = frame->get_code();
        frame->m_pc = pc;

        Value *registers = frame->registers();

        while (true) {
            bytecode::Opcode op = static_cast<bytecode::Opcode>(*pc++);
            switch (op) {
                case bytecode::Opcode::LoadImmInt: {
                    bytecode::LoadImmInt instr = frame->read_at<bytecode::LoadImmInt>(pc);
                    registers[instr.reg] = Value(instr.imm);
                    break;
                }
                case bytecode::Opcode::Move: {
                    bytecode::Move instr = frame->read_at<bytecode::Move>(pc);
                    registers[instr.dst] = registers[instr.src];
                    break;
                }
                case bytecode::Opcode::Neg: {
                    bytecode::Neg instr = frame->read_at<bytecode::Neg>(pc);
                    registers[instr.result] = Handlers::handle_neg(registers[instr.arg]);
                    break;
                }
                case bytecode::Opcode::Not: {
                    bytecode::Not instr = frame->read_at<bytecode::Not>(pc);
                    registers[instr.result] = Handlers::handle_not(registers[instr.arg]);
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
                case bytecode::Opcode::Lt: {
                    bytecode::Lt instr = frame->read_at<bytecode::Lt>(pc);
                    registers[instr.reg] =
                            Handlers::handle_lt(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Lte: {
                    bytecode::Lte instr = frame->read_at<bytecode::Lte>(pc);
                    registers[instr.reg] =
                            Handlers::handle_lte(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Gt: {
                    bytecode::Gt instr = frame->read_at<bytecode::Gt>(pc);
                    registers[instr.reg] =
                            Handlers::handle_gt(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Gte: {
                    bytecode::Gte instr = frame->read_at<bytecode::Gte>(pc);
                    registers[instr.reg] =
                            Handlers::handle_gte(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Eq: {
                    bytecode::Eq instr = frame->read_at<bytecode::Eq>(pc);
                    registers[instr.reg] =
                            Handlers::handle_eq(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Neq: {
                    bytecode::Neq instr = frame->read_at<bytecode::Neq>(pc);
                    registers[instr.reg] =
                            Handlers::handle_neq(registers[instr.lhs], registers[instr.rhs]);
                    break;
                }
                case bytecode::Opcode::Jmp: {
                    bytecode::Jmp instr = frame->read_at<bytecode::Jmp>(pc);
                    pc = frame->get_code() + instr.offset;
                    break;
                }
                case bytecode::Opcode::JmpIfFalse: {
                    bytecode::JmpIfFalse instr = frame->read_at<bytecode::JmpIfFalse>(pc);

                    if (!registers[instr.arg].is_truthy()) {
                        pc = frame->get_code() + instr.offset;
                    }

                    break;
                }
                case bytecode::Opcode::JmpIfTrue: {
                    bytecode::JmpIfTrue instr = frame->read_at<bytecode::JmpIfTrue>(pc);

                    if (registers[instr.arg].is_truthy()) {
                        pc = frame->get_code() + instr.offset;
                    }

                    break;
                }
                case bytecode::Opcode::Ret: {
                    bytecode::Ret instr = frame->read_at<bytecode::Ret>(pc);
                    return registers[instr.reg];
                }
                case bytecode::Opcode::RetNil: {
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
