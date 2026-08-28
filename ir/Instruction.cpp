#include "ir/Instruction.h"
#include <cstdint>
#include "ir/BasicBlock.h"

namespace Nyx::ir {
    void LoadImmInt::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::LoadImmInt));
        buffer.push_back(static_cast<uint8_t>(m_value));
        buffer.push_back(static_cast<uint8_t>(m_value >> 8));
        buffer.push_back(static_cast<uint8_t>(m_value >> 16));
        buffer.push_back(static_cast<uint8_t>(m_value >> 24));
        buffer.push_back(static_cast<uint8_t>(m_value >> 32));
        buffer.push_back(static_cast<uint8_t>(m_value >> 40));
        buffer.push_back(static_cast<uint8_t>(m_value >> 48));
        buffer.push_back(static_cast<uint8_t>(m_value >> 56));
    }

    void LoadConst::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::LoadConst));
        buffer.push_back(static_cast<uint8_t>(m_idx));
        buffer.push_back(static_cast<uint8_t>(m_idx >> 8));
    }

    void LoadString::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::LoadString));
        buffer.push_back(static_cast<uint8_t>(m_idx));
        buffer.push_back(static_cast<uint8_t>(m_idx >> 8));
    }

    void Pop::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Pop));
    }

    void StoreLocal::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::StoreLocal));
        buffer.push_back(static_cast<uint8_t>(m_slot));
    }

    void GetLocal::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::GetLocal));
        buffer.push_back(static_cast<uint8_t>(m_slot));
    }

    void GetGlobalFast::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::GetGlobalFast));
        buffer.push_back(static_cast<uint8_t>(m_slot));
        buffer.push_back(static_cast<uint8_t>(m_slot >> 8));
    }

    void GetGlobalUnresolved::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::GetGlobalUnresolved));
        buffer.push_back(static_cast<uint8_t>(m_slot));
        buffer.push_back(static_cast<uint8_t>(m_slot >> 8));
    }

    void Not::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Not));
    }

    void Neg::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Neg));
    }

    void Add::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Add));
    }

    void Sub::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Sub));
    }

    void Mul::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Mul));
    }

    void Div::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Div));
    }

    void Mod::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Mod));
    }

    void Lt::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Lt));
    }

    void Lte::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Lte));
    }

    void Gt::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Gt));
    }

    void Gte::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Gte));
    }

    void Eq::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Eq));
    }

    void Neq::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Neq));
    }

    void JmpIfFalse::lower(std::vector<uint8_t> &buffer) {
        // TODO: should convert from absolute offset to relative offset.
        uint16_t offset = static_cast<uint16_t>(m_target->code_offset());
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::JmpIfFalse));
        buffer.push_back(static_cast<uint8_t>(offset));
        buffer.push_back(static_cast<uint8_t>(offset >> 8));
    }

    void JmpIfTrue::lower(std::vector<uint8_t> &buffer) {
        // TODO: should convert from absolute offset to relative offset.
        uint16_t offset = static_cast<uint16_t>(m_target->code_offset());
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::JmpIfTrue));
        buffer.push_back(static_cast<uint8_t>(offset));
        buffer.push_back(static_cast<uint8_t>(offset >> 8));
    }

    void Jmp::lower(std::vector<uint8_t> &buffer) {
        // TODO: should convert from absolute offset to relative offset.
        uint16_t offset = static_cast<uint16_t>(m_target->code_offset());
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Jmp));
        buffer.push_back(static_cast<uint8_t>(offset));
        buffer.push_back(static_cast<uint8_t>(offset >> 8));
    }

    void Branch::lower(std::vector<uint8_t> &buffer) {
        uint16_t true_offset = static_cast<uint16_t>(m_true_target->code_offset());
        uint16_t false_offset = static_cast<uint16_t>(m_false_target->code_offset());
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Branch));
        buffer.push_back(static_cast<uint8_t>(true_offset));
        buffer.push_back(static_cast<uint8_t>(true_offset >> 8));
        buffer.push_back(static_cast<uint8_t>(false_offset));
        buffer.push_back(static_cast<uint8_t>(false_offset >> 8));
    }

    void Call::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Call));
        buffer.push_back(static_cast<uint8_t>(m_arg_count));
    }

    void Ret::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::Ret));
    }
    void RetNil::lower(std::vector<uint8_t> &buffer) {
        buffer.push_back(static_cast<uint8_t>(bytecode::Opcode::RetNil));
    }

} // namespace Nyx::ir
