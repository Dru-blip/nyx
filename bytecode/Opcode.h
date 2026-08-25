#pragma once

#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        LoadConst,
        LoadString,


        StoreLocal,
        GetLocal,

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

        Ret,
        RetNil,

        Call,
    };
} // namespace Nyx::bytecode
