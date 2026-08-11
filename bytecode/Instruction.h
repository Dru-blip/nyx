#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        Ret,
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
    };

#pragma pack(push, 1)
    struct Ret {
        uint8_t reg;
    };

    struct LoadImmInt {
        uint8_t reg;
        int64_t imm;
    };

#define DefineUnaryInst(name)                                                                      \
    struct name {                                                                                  \
        uint8_t arg;                                                                               \
        uint8_t result;                                                                            \
    };

    DefineUnaryInst(Not);
    DefineUnaryInst(Neg);

#define DefineBinaryInst(name)                                                                     \
    struct name {                                                                                  \
        uint8_t lhs;                                                                               \
        uint8_t rhs;                                                                               \
        uint8_t reg;                                                                               \
    };

    DefineBinaryInst(Add);
    DefineBinaryInst(Sub);
    DefineBinaryInst(Mul);
    DefineBinaryInst(Div);

    DefineBinaryInst(Lt);
    DefineBinaryInst(Lte);
    DefineBinaryInst(Gt);
    DefineBinaryInst(Gte);
    DefineBinaryInst(Eq);
    DefineBinaryInst(Neq);
#pragma pack(pop)
} // namespace Nyx::bytecode
