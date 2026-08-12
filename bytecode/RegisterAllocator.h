#pragma once

#include <bitset>
#include <cstdint>

namespace Nyx::bytecode {
    class RegisterAllocator {
    public:
        static constexpr uint8_t MaxRegisters = 255;

        RegisterAllocator() {}

        uint8_t allocate();
        void free(uint8_t reg);

    private:
        uint8_t m_max_used{};
        std::bitset<MaxRegisters> m_used_registers{};
    };
} // namespace Nyx::bytecode
