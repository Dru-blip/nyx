

#include "bytecode/Executable.h"
#include <print>
#include "bytecode/Instruction.h"

namespace Nyx::bytecode {
    void Executable::print_code() {
        const size_t size = m_code.size();
        size_t offset = 0;
        while (offset < size) {
            Opcode *op = reinterpret_cast<Opcode *>(m_code.data() + offset);
            switch (*op) {
                case Opcode::RetImm: {
                    RetImm *ret = reinterpret_cast<RetImm *>(m_code.data() + offset);
                    std::println("[{}] RetImm ${}", offset, ret->value);
                    offset += sizeof(RetImm);
                    break;
                }
                case Opcode::Ret: {
                    // Ret *ret = reinterpret_cast<Ret *>(m_code.data() + offset);
                    std::println("[{}] Ret", offset);
                    offset += sizeof(Ret);
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
