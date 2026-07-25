#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LdaImmInt,
        Ret,
        RetImm,
    };

    struct LoadImmInt {
        Opcode opcode;
        int32_t value;
    };

    struct Ret {
        Opcode opcode;
    };

    struct RetImm {
        Opcode opcode;
        int64_t value;
    };
} // namespace Nyx::bytecode
