#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include "bytecode/Operand.h"
#include "bytecode/RegisterAllocator.h"


namespace Nyx::bytecode {
    class InstructionEmitter {
    public:
        template<typename InstType>
        void emit(const InstType &inst) {
            m_code.resize(m_code.size() + sizeof(InstType));
            std::memcpy(m_code.data() + m_code.size() - sizeof(InstType), &inst, sizeof(InstType));
        }

        void push(const uint8_t byte) { m_code.push_back(byte); }
        void push_bytes(const void *start, const size_t size) {
            const uint8_t *ptr = static_cast<const uint8_t *>(start);
            m_code.insert(m_code.end(), ptr, ptr + size);
        }

        std::vector<uint8_t> &code() { return m_code; }
        void ret(const uint8_t reg);
        uint8_t load_imm_int(const int64_t imm);
        uint8_t add(const uint8_t &left, const uint8_t &right);
        uint8_t sub(const uint8_t &left, const uint8_t &right);
        uint8_t mul(const uint8_t &left, const uint8_t &right);
        uint8_t div(const uint8_t &left, const uint8_t &right);


    private:
        std::vector<uint8_t> m_code;
        RegisterAllocator m_register_allocator;
    };
} // namespace Nyx::bytecode
