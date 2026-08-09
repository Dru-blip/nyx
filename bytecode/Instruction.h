#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LdaImmInt,
        Ret,
        RetImm,
        Add,
    };

#pragma pack(push, 1)
    struct RetImm {
        Opcode opcode = Opcode::RetImm;
        int64_t imm;
    };
#pragma pack(pop)

} // namespace Nyx::bytecode
