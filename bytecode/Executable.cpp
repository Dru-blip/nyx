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
                case Opcode::Add: {
                    Add add;
                    std::memcpy(&add, pc, sizeof(Add));
                    std::println("[{}] Add %{} = %{} + %{}", pc - m_code.data(), add.reg, add.lhs,
                                 add.rhs);
                    pc += sizeof(Add);
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
