#pragma once

#include <cstdint>

namespace Nyx::bytecode {
    enum class OperandType {
        ConstInt,
        Register,
    };

    union OperandValue {
        int64_t imm;
        uint8_t reg;
    };

    struct Operand {
        OperandType type;
        OperandValue as;

        Operand(OperandType type, OperandValue as) : type(type), as(as) {}

        bool isConstInt() const { return type == OperandType::ConstInt; }
        bool isRegister() const { return type == OperandType::Register; }
    };
} // namespace Nyx::bytecode
