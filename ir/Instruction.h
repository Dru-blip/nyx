#include <cstdint>

#include "bytecode/InstructionEmitter.h"
#include "ir/Register.h"

namespace Nyx::ir {
    class BasicBlock;

    using InstructionEmitter = bytecode::InstructionEmitter;

    class Instruction {
    public:
        virtual ~Instruction() = default;
        virtual bool is_terminator() const { return false; }
        virtual void lower(InstructionEmitter &emitter) = 0;
    };

    class LoadImmInt : public Instruction {
    public:
        LoadImmInt(int64_t value, Register reg) : m_value(value), m_reg(reg) {}
        void lower(InstructionEmitter &emitter) override;

    private:
        int64_t m_value;
        Register m_reg;
    };

    class Move : public Instruction {
    public:
        Move(Register src, Register dst) : m_src(src), m_dst(dst) {}
        void lower(InstructionEmitter &emitter) override;

    private:
        Register m_src, m_dst;
    };

    class Unary : public Instruction {
    public:
        Unary(Register value, Register dst) : m_value(value), m_dst(dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;


    protected:
        Register m_value, m_dst;
    };

    class Not : public Unary {
    public:
        Not(Register value, Register dst) : Unary(value, dst) {}
        void lower(InstructionEmitter &emitter) override;
    };

    class Neg : public Unary {
    public:
        Neg(Register value, Register dst) : Unary(value, dst) {}
        void lower(InstructionEmitter &emitter) override;
    };

    class Binary : public Instruction {
    public:
        Binary(Register lhs, Register rhs, Register dst) : m_lhs(lhs), m_rhs(rhs), m_dst(dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;

    protected:
        Register m_lhs, m_rhs, m_dst;
    };


    class Add : public Binary {
    public:
        Add(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(InstructionEmitter &emitter) override;
    };

    class Sub : public Binary {
    public:
        Sub(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Mul : public Binary {
    public:
        Mul(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Div : public Binary {
    public:
        Div(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Lt : public Binary {
    public:
        Lt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Lte : public Binary {
    public:
        Lte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Gt : public Binary {
    public:
        Gt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Gte : public Binary {
    public:
        Gte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Eq : public Binary {
    public:
        Eq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Neq : public Binary {
    public:
        Neq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class JmpIfFalse : public Instruction {
    public:
        JmpIfFalse(Register condition, BasicBlock *target) : condition(condition), target(target) {}
        bool is_terminator() const override { return true; }
        void lower(bytecode::InstructionEmitter &emitter) override;

    private:
        Register condition;
        BasicBlock *target;
    };

    class JmpIfTrue : public Instruction {
    public:
        JmpIfTrue(Register condition, BasicBlock *target) : condition(condition), target(target) {}
        bool is_terminator() const override { return true; }
        void lower(bytecode::InstructionEmitter &emitter) override;

    private:
        Register condition;
        BasicBlock *target;
    };


    class Ret : public Instruction {
    public:
        Ret(Register value) : m_value(value) {}
        bool is_terminator() const override { return true; }
        void lower(bytecode::InstructionEmitter &emitter) override;

    private:
        Register m_value;
    };


} // namespace Nyx::ir
