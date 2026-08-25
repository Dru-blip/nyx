#include "bytecode/Executable.h"
#include <cstdint>
#include <print>
#include "bytecode/Opcode.h"

namespace Nyx::bytecode {
    static uint16_t read_u16(const uint8_t *&pc) {
        uint16_t val = static_cast<uint16_t>(pc[0]) | (static_cast<uint16_t>(pc[1]) << 8);
        pc += 2;
        return val;
    }

    static int64_t read_i64(const uint8_t *&pc) {
        uint64_t val = 0;
        for (int i = 0; i < 8; ++i) {
            val |= (static_cast<uint64_t>(pc[i]) << (i * 8));
        }
        pc += 8;
        return static_cast<int64_t>(val);
    }

    static uint8_t read_u8(const uint8_t *&pc) { return *pc++; }

    void Executable::print_code() {
        // AI:
        std::println("Max Stack Size : {}", m_stack_size);
        std::println("Constants Count: {}", m_constants.size());
        std::println("Code Size      : {} bytes", m_code.size());

        const uint8_t *start = m_code.data();
        const uint8_t *pc = start;
        const uint8_t *end = pc + m_code.size();

        while (pc < end) {
            std::size_t offset = pc - start;
            Opcode op = static_cast<Opcode>(*pc++);

            switch (op) {
                case Opcode::Ret: {
                    std::println("[{}] Ret", offset);
                    break;
                }
                case Opcode::RetNil: {
                    std::println("[{}] RetNil", offset);
                    break;
                }
                case Opcode::LoadImmInt: {
                    int64_t imm = read_i64(pc);
                    std::println("[{}] LoadImmInt  {}", offset, imm);
                    break;
                }
                case Opcode::LoadConst: {
                    uint16_t idx = read_u16(pc);
                    std::println("[{}] LoadConst   {}", offset, idx);
                    break;
                }
                case Opcode::GetLocal: {
                    uint8_t slot = read_u8(pc);
                    std::println("[{}] GetLocal    {}", offset, slot);
                    break;
                }
                case Opcode::StoreLocal: {
                    uint8_t slot = read_u8(pc);
                    std::println("[{}] StoreLocal  {}", offset, slot);
                    break;
                }
                case Opcode::Neg:
                    std::println("[{}] Neg", offset);
                    break;
                case Opcode::Not:
                    std::println("[{}] Not", offset);
                    break;
                case Opcode::Add:
                    std::println("[{}] Add", offset);
                    break;
                case Opcode::Sub:
                    std::println("[{}] Sub", offset);
                    break;
                case Opcode::Mul:
                    std::println("[{}] Mul", offset);
                    break;
                case Opcode::Div:
                    std::println("[{}] Div", offset);
                    break;
                case Opcode::Lt:
                    std::println("[{}] Lt", offset);
                    break;
                case Opcode::Lte:
                    std::println("[{}] Lte", offset);
                    break;
                case Opcode::Gt:
                    std::println("[{}] Gt", offset);
                    break;
                case Opcode::Gte:
                    std::println("[{}] Gte", offset);
                    break;
                case Opcode::Eq:
                    std::println("[{}] Eq", offset);
                    break;
                case Opcode::Neq:
                    std::println("[{}] Neq", offset);
                    break;

                case Opcode::Jmp: {
                    uint16_t target = read_u16(pc);
                    std::println("[{}] Jmp {}", offset, target);
                    break;
                }
                case Opcode::JmpIfFalse: {
                    uint16_t target = read_u16(pc);
                    std::println("[{}] JmpIfFalse {}", offset, target);
                    break;
                }
                case Opcode::JmpIfTrue: {
                    uint16_t target = read_u16(pc);
                    std::println("[{}] JmpIfTrue {}", offset, target);
                    break;
                }
                case Opcode::Branch: {
                    uint16_t true_target = read_u16(pc);
                    uint16_t false_target = read_u16(pc);
                    std::println("[{}] Branch true:{}, false:{}", offset, true_target,
                                 false_target);
                    break;
                }
                case Opcode::Call: {
                    uint8_t arg_count = read_u8(pc);
                    std::println("[{}] Call {}", offset, arg_count);
                    break;
                }
                default: {
                    std::println("[{}] Unknown Opcode: {:#x}", offset, static_cast<uint8_t>(op));
                    break;
                }
            }
        }
    }

} // namespace Nyx::bytecode
