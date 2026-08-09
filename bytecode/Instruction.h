#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        Ret,
        Add,
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
#pragma pack(pop)

} // namespace Nyx::bytecode
