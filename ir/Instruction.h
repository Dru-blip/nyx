#pragma once
#include <cstdint>

#include "bytecode/Instruction.h"
#include "bytecode/InstructionEmitter.h"
#include "ir/Register.h"

namespace Nyx::ir {
    class BasicBlock;

    using InstructionEmitter = bytecode::InstructionEmitter;

    class Instruction {
    public:
        virtual ~Instruction() = default;
        virtual void lower(InstructionEmitter &emitter) = 0;
        virtual bool is_terminator() const { return false; }
        virtual constexpr std::size_t length() const { return 0; }
    };

    class BlockTerminator : public Instruction {
    public:
        virtual ~BlockTerminator() = default;
        bool is_terminator() const override { return true; }
    };

    class LoadImmInt : public Instruction {
    public:
        LoadImmInt(int64_t value, Register reg) : m_value(value), m_reg(reg) {}
        constexpr std::size_t length() const override { return sizeof(bytecode::LoadImmInt); }
        void lower(InstructionEmitter &emitter) override;

    private:
        int64_t m_value;
        Register m_reg;
    };

    class Move : public Instruction {
    public:
        Move(Register src, Register dst) : m_src(src), m_dst(dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Move); }

    private:
        Register m_src, m_dst;
    };

    class Unary : public Instruction {
    public:
        Unary(Register value, Register dst) : m_value(value), m_dst(dst) {}
        constexpr std::size_t length() const override { return sizeof(bytecode::Not); }

    protected:
        Register m_value, m_dst;
    };

    class Not : public Unary {
    public:
        Not(Register value, Register dst) : Unary(value, dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Not); }
    };

    class Neg : public Unary {
    public:
        Neg(Register value, Register dst) : Unary(value, dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Neg); }
    };

    class Binary : public Instruction {
    public:
        Binary(Register lhs, Register rhs, Register dst) : m_lhs(lhs), m_rhs(rhs), m_dst(dst) {}

    protected:
        Register m_lhs, m_rhs, m_dst;
    };


    class Add : public Binary {
    public:
        Add(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Add); }
    };

    class Sub : public Binary {
    public:
        Sub(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Sub); }
    };

    class Mul : public Binary {
    public:
        Mul(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Mul); }
    };

    class Div : public Binary {
    public:
        Div(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Div); }
    };

    class Lt : public Binary {
    public:
        Lt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Lt); }
    };

    class Lte : public Binary {
    public:
        Lte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Lte); }
    };

    class Gt : public Binary {
    public:
        Gt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Gt); }
    };

    class Gte : public Binary {
    public:
        Gte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Gte); }
    };

    class Eq : public Binary {
    public:
        Eq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Eq); }
    };

    class Neq : public Binary {
    public:
        Neq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Neq); }
    };

    class JmpIfFalse : public BlockTerminator {
    public:
        JmpIfFalse(Register condition, BasicBlock *target) :
            m_condition(condition), m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;

        constexpr std::size_t length() const override { return sizeof(bytecode::JmpIfFalse); }

    private:
        Register m_condition;
        BasicBlock *m_target;
    };

    class JmpIfTrue : public BlockTerminator {
    public:
        JmpIfTrue(Register condition, BasicBlock *target) :
            m_condition(condition), m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::JmpIfTrue); }

    private:
        Register m_condition;
        BasicBlock *m_target;
    };


    class Ret : public BlockTerminator {
    public:
        Ret(Register value) : m_value(value) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Ret); }

    private:
        Register m_value;
    };


} // namespace Nyx::ir
