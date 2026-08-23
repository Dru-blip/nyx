#pragma once

#include <bitset>
#include <cstdint>
#include "ir/Register.h"

namespace Nyx::ir {
    class RegisterAllocator {
    public:
        static constexpr uint8_t MaxRegisters = 255;

        RegisterAllocator() {}

        uint8_t max_used() const { return m_max_used; }

        Register allocate();
        Register allocate_local(){
            return Register(m_local_start++);
        };
        void free(Register reg);

    private:
        uint8_t m_max_used{};
        uint8_t m_local_start{};
        std::bitset<MaxRegisters> m_used_registers{};
    };
} // namespace Nyx::ir
