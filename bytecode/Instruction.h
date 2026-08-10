#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        Ret,
        Add,
        Sub,
        Mul,
        Div,
    };

#pragma pack(push, 1)
    struct Ret {
        uint8_t reg;
    };

    struct LoadImmInt {
        uint8_t reg;
        int64_t imm;
    };

    struct Add {
        uint8_t lhs;
        uint8_t rhs;
        uint8_t reg;
    };

    struct Sub {
        uint8_t lhs;
        uint8_t rhs;
        uint8_t reg;
    };

    struct Mul {
        uint8_t lhs;
        uint8_t rhs;
        uint8_t reg;
    };

    struct Div {
        uint8_t lhs;
        uint8_t rhs;
        uint8_t reg;
    };
#pragma pack(pop)

} // namespace Nyx::bytecode
