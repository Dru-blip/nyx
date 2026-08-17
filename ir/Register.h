
#pragma once

#include <cstdint>

namespace Nyx::ir {
    class Register {
        enum class Type : uint8_t {
            Var,
            Temp,
        };

    public:
        explicit Register(uint8_t slot) : m_slot(slot) {}

        uint8_t slot() const { return m_slot; }
        bool isVar() const { return m_type == Type::Var; }
        bool isTemp() const { return m_type == Type::Temp; }
        Type type() const { return m_type; }


    private:
        uint8_t m_slot;
        Type m_type;
    };
} // namespace Nyx::ir
