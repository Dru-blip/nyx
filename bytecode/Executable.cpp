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
            Opcode op = static_cast<Opcode>(*pc);
            switch (op) {
                case Opcode::RetImm: {
                    RetImm ret;
                    std::memcpy(&ret, pc, sizeof(RetImm));
                    std::println("[{}] RetImm ${}", pc - m_code.data(), ret.imm);
                    pc += sizeof(RetImm);
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
