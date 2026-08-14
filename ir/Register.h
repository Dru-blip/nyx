
#pragma once

#include <cstdint>

namespace Nyx::ir {
    class Register {
    public:
        explicit Register(uint8_t slot) : m_slot(slot) {}

        uint8_t slot() const { return m_slot; }

    private:
        uint8_t m_slot;
    };
} // namespace Nyx::ir
