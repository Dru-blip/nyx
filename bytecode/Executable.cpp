#include "bytecode/Executable.h"
#include <cstdint>
#include <cstring>
#include <print>
#include "bytecode/Instruction.h"

namespace Nyx::bytecode {
    void Executable::print_code() {
        const std::size_t size = m_code.size();
        uint8_t *pc = m_code.data();
        const uint8_t *end = pc + size;
        while (pc < end) {
            Opcode op = static_cast<Opcode>(*pc++);
            switch (op) {
                case Opcode::Ret: {
                    Ret ret;
                    std::memcpy(&ret, pc, sizeof(Ret));
                    std::println("[{}] Ret %{}", pc - m_code.data(), ret.reg);
                    pc += sizeof(Ret);
                    break;
                }
                case Opcode::LoadImmInt: {
                    LoadImmInt load;
                    std::memcpy(&load, pc, sizeof(LoadImmInt));
                    std::println("[{}] LoadImmInt %{} = {}", pc - m_code.data(), load.reg,
                                 load.imm);
                    pc += sizeof(LoadImmInt);
                    break;
                }
                case Opcode::Move: {
                    Move move;
                    std::memcpy(&move, pc, sizeof(Move));
                    std::println("[{}] Move %{} = %{}", pc - m_code.data(), move.dst, move.src);
                    pc += sizeof(Move);
                    break;
                }
                case Opcode::Neg: {
                    print_unary_instruction<Neg>("Neg", "-", pc);
                    break;
                }
                case Opcode::Not: {
                    print_unary_instruction<Not>("Not", "!", pc);
                    break;
                }
                case Opcode::Add: {
                    print_binary_instruction<Add>("Add", "+", pc);
                    break;
                }
                case Opcode::Sub: {
                    print_binary_instruction<Sub>("Sub", "-", pc);
                    break;
                }
                case Opcode::Mul: {
                    print_binary_instruction<Mul>("Mul", "*", pc);
                    break;
                }
                case Opcode::Div: {
                    print_binary_instruction<Div>("Div", "/", pc);
                    break;
                }
                case Opcode::Lt: {
                    print_binary_instruction<Lt>("Lt", "<", pc);
                    break;
                }
                case Opcode::Gt: {
                    print_binary_instruction<Gt>("Gt", ">", pc);
                    break;
                }
                case Opcode::Lte: {
                    print_binary_instruction<Lte>("Lte", "<=", pc);
                    break;
                }
                case Opcode::Gte: {
                    print_binary_instruction<Gte>("Gte", ">=", pc);
                    break;
                }
                case Opcode::Eq: {
                    print_binary_instruction<Eq>("Eq", "==", pc);
                    break;
                }
                case Opcode::Neq: {
                    print_binary_instruction<Neq>("Neq", "!=", pc);
                    break;
                }

                case Opcode::Jmp: {
                    Jmp inst;
                    std::memcpy(&inst, pc, sizeof(Jmp));
                    std::println("[{}] Jmp {}", pc - m_code.data(),
                                 inst.offset);
                    pc += sizeof(Jmp);
                    break;
                }

                case Opcode::JmpIfFalse: {
                    JmpIfFalse inst;
                    std::memcpy(&inst, pc, sizeof(JmpIfFalse));
                    std::println("[{}] JmpIfFalse %{} {}", pc - m_code.data(), inst.arg,
                                 inst.offset);
                    pc += sizeof(JmpIfFalse);
                    break;
                }
                case Opcode::JmpIfTrue: {
                    JmpIfTrue inst;
                    std::memcpy(&inst, pc, sizeof(JmpIfTrue));
                    std::println("[{}] JmpIfTrue %{} {}", pc - m_code.data(), inst.arg,
                                 inst.offset);
                    pc += sizeof(JmpIfTrue);
                    break;
                }
                default: {
                    std::println("Unknown opcode");
                    break;
                }
            }
        }
    }
} // namespace Nyx::bytecode
