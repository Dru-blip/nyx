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
        void free(Register reg);

    private:
        uint8_t m_max_used{};
        std::bitset<MaxRegisters> m_used_registers{};
    };
} // namespace Nyx::ir
