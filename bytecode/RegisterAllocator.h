#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    class RegisterAllocator {
    public:
        RegisterAllocator() {}

        uint8_t allocate();
        void free(uint8_t reg);

    private:
        uint8_t m_next_register{};
        uint8_t m_max_registers{255};
        uint8_t m_max_used{};
    };
} // namespace Nyx::bytecode
