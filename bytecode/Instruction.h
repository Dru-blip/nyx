#pragma once


#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        Move,

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

        JmpIfFalse,
        JmpIfFalseMove,
    };

#pragma pack(push, 1)
    struct Ret {
        uint8_t reg;
        Ret(uint8_t reg) : reg(reg) {}
    };

    struct LoadImmInt {
        uint8_t reg;
        int64_t imm;
        LoadImmInt(uint8_t reg, int64_t imm) : reg(reg), imm(imm) {}
    };

    struct Move {
        uint8_t src;
        uint8_t dst;
        Move(uint8_t src, uint8_t dst) : src(src), dst(dst) {}
    };

#define DefineUnaryInst(name)                                                                      \
    struct name {                                                                                  \
        uint8_t arg;                                                                               \
        uint8_t result;                                                                            \
        name(uint8_t arg, uint8_t result) : arg(arg), result(result) {}                            \
    };

    DefineUnaryInst(Not);
    DefineUnaryInst(Neg);

#define DefineBinaryInst(name)                                                                     \
    struct name {                                                                                  \
        uint8_t lhs;                                                                               \
        uint8_t rhs;                                                                               \
        uint8_t reg;                                                                               \
        name(uint8_t lhs, uint8_t rhs, uint8_t reg) : lhs(lhs), rhs(rhs), reg(reg) {}              \
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


    struct JmpIfFalse {
        uint8_t arg;
        uint16_t offset;

        JmpIfFalse(uint8_t arg, uint16_t offset) : arg(arg), offset(offset) {}
    };

    struct JmpIfFalseMove {
        uint8_t arg;
        uint8_t result;
        uint16_t offset;

        JmpIfFalseMove(uint8_t arg, uint8_t result, uint16_t offset) :
            arg(arg), result(result), offset(offset) {}
    };

#pragma pack(pop)
} // namespace Nyx::bytecode
