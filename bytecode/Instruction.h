#pragma once

#include <cstdint>

namespace Nyx::bytecode {
    enum class Opcode : uint8_t {
        LoadImmInt,
        LoadConst,
        LoadString,

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
        Ret() = default;
    };

    struct LoadImmInt {
        int64_t imm;
        LoadImmInt() = default;
        LoadImmInt(int64_t imm) : imm(imm) {}
    };

    struct LoadConst {
        uint16_t idx;
        LoadConst() = default;
        LoadConst(uint16_t idx) : idx(idx) {}
    };

    struct LoadString {
        uint16_t idx;
        LoadString() = default;
        LoadString(uint16_t idx) : idx(idx) {}
    };


#define DefineUnaryInst(name)                                                                      \
    struct name {                                                                                  \
        name() = default;                                                                          \
    };

    DefineUnaryInst(Not);
    DefineUnaryInst(Neg);

#define DefineBinaryInst(name)                                                                     \
    struct name {};

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
        uint16_t offset;

        JmpIfFalse() = default;
        JmpIfFalse(uint16_t offset) : offset(offset) {}
    };

    struct JmpIfTrue {
        uint16_t offset;

        JmpIfTrue() = default;
        JmpIfTrue(uint16_t offset) : offset(offset) {}
    };

    struct Call {
        uint8_t arg_count;


        Call() = default;
        Call(uint8_t arg_count) : arg_count(arg_count) {}
    };


#pragma pack(pop)
} // namespace Nyx::bytecode
