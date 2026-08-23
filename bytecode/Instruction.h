#pragma once

#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        LoadConst,
        LoadString,
        Move,

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
        Ret,
        RetNil,

        Call,
    };

#pragma pack(push, 1)
    struct Ret {
        uint8_t reg;
        Ret() = default;
        Ret(uint8_t reg) : reg(reg) {}
    };

    struct LoadImmInt {
        uint8_t reg;
        int64_t imm;
        LoadImmInt() = default;
        LoadImmInt(uint8_t reg, int64_t imm) : reg(reg), imm(imm) {}
    };

    struct LoadConst {
        uint8_t reg;
        uint16_t idx;
        LoadConst() = default;
        LoadConst(uint8_t reg, uint16_t idx) : reg(reg), idx(idx) {}
    };

    struct LoadString {
        uint8_t reg;
        uint16_t idx;
        LoadString() = default;
        LoadString(uint8_t reg, uint16_t idx) : reg(reg), idx(idx) {}
    };

    struct Move {
        uint8_t src;
        uint8_t dst;
        Move() = default;
        Move(uint8_t src, uint8_t dst) : src(src), dst(dst) {}
    };

#define DefineUnaryInst(name)                                                                      \
    struct name {                                                                                  \
        uint8_t arg;                                                                               \
        uint8_t result;                                                                            \
        name() = default;                                                                          \
        name(uint8_t arg, uint8_t result) : arg(arg), result(result) {}                            \
    };

    DefineUnaryInst(Not);
    DefineUnaryInst(Neg);

#define DefineBinaryInst(name)                                                                     \
    struct name {                                                                                  \
        uint8_t lhs;                                                                               \
        uint8_t rhs;                                                                               \
        uint8_t reg;                                                                               \
        name() = default;                                                                          \
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

    struct Jmp {
        uint16_t offset;
        Jmp() = default;
        Jmp(uint16_t offset) : offset(offset) {}
    };

    struct JmpIfFalse {
        uint8_t arg;
        uint16_t offset;

        JmpIfFalse() = default;
        JmpIfFalse(uint8_t arg, uint16_t offset) : arg(arg), offset(offset) {}
    };

    struct JmpIfTrue {
        uint8_t arg;
        uint16_t offset;

        JmpIfTrue() = default;
        JmpIfTrue(uint8_t arg, uint16_t offset) : arg(arg), offset(offset) {}
    };

    struct Call {
        uint8_t callee;
        uint8_t arg_count;
        uint8_t result;


        Call() = default;
        Call(uint8_t callee, uint8_t arg_count, uint8_t result) :
            callee(callee), arg_count(arg_count), result(result) {}
    };


#pragma pack(pop)
} // namespace Nyx::bytecode
