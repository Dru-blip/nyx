#pragma once

#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        LoadConst,
        LoadString,

        Pop,

        StoreLocal,
        GetLocal,
        GetGlobalFast,
        GetGlobalUnresolved,

        Not,
        Neg,
        Add,
        Sub,
        Mul,
        Div,
        Lt,
        Lte,

        Gt,
        Gte,
        Eq,
        Neq,

        Jmp,
        JmpIfFalse,
        JmpIfTrue,
        Branch,

        Call,
        Ret,
        RetNil,
    };
} // namespace Nyx::bytecode
