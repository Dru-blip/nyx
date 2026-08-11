#include "bytecode/Executable.h"
#include <cstdint>
#include <print>
#include "bytecode/Instruction.h"

namespace Nyx::bytecode {
    void Executable::print_code() {
        const size_t size = m_code.size();
        const uint8_t *pc = m_code.data();
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
                case Opcode::Neg: {
                    Neg neg;
                    std::memcpy(&neg, pc, sizeof(Neg));
                    std::println("[{}] Neg %{} = -%{}", pc - m_code.data(), neg.result, neg.arg);
                    pc += sizeof(Neg);
                    break;
                }
                case Opcode::Not: {
                    Not not_;
                    std::memcpy(&not_, pc, sizeof(Not));
                    std::println("[{}] Not %{} = !%{}", pc - m_code.data(), not_.result, not_.arg);
                    pc += sizeof(Not);
                    break;
                }
                case Opcode::Add: {
                    Add add;
                    std::memcpy(&add, pc, sizeof(Add));
                    std::println("[{}] Add %{} = %{} + %{}", pc - m_code.data(), add.reg, add.lhs,
                                 add.rhs);
                    pc += sizeof(Add);
                    break;
                }
                case Opcode::Sub: {
                    Sub sub;
                    std::memcpy(&sub, pc, sizeof(Sub));
                    std::println("[{}] Sub %{} = %{} - %{}", pc - m_code.data(), sub.reg, sub.lhs,
                                 sub.rhs);
                    pc += sizeof(Sub);
                    break;
                }
                case Opcode::Mul: {
                    Mul mul;
                    std::memcpy(&mul, pc, sizeof(Mul));
                    std::println("[{}] Mul %{} = %{} * %{}", pc - m_code.data(), mul.reg, mul.lhs,
                                 mul.rhs);
                    pc += sizeof(Mul);
                    break;
                }
                case Opcode::Div: {
                    Div div;
                    std::memcpy(&div, pc, sizeof(Div));
                    std::println("[{}] Div %{} = %{} / %{}", pc - m_code.data(), div.reg, div.lhs,
                                 div.rhs);
                    pc += sizeof(Div);
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
