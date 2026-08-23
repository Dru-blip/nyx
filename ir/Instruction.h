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
        virtual constexpr std::size_t length() const = 0;
    };

    class BlockTerminator : public Instruction {
    public:
        virtual ~BlockTerminator() = default;
        bool is_terminator() const override { return true; }
    };

    class LoadImmInt : public Instruction {
    public:
        LoadImmInt(int64_t value, Register reg) : m_value(value), m_reg(reg) {}
        constexpr std::size_t length() const override {
            return sizeof(bytecode::LoadImmInt) + sizeof(bytecode::Opcode);
        }
        void lower(InstructionEmitter &emitter) override;

    private:
        int64_t m_value;
        Register m_reg;
    };

    class LoadConst : public Instruction {
    public:
        LoadConst(uint16_t idx, Register reg) : m_idx(idx), m_reg(reg) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::LoadConst) + sizeof(bytecode::Opcode);
        }

    private:
        uint16_t m_idx;
        Register m_reg;
    };

    class LoadString : public Instruction {
    public:
        LoadString(uint16_t idx, Register reg) : m_idx(idx), m_reg(reg) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::LoadString) + sizeof(bytecode::Opcode);
        }

    private:
        uint16_t m_idx;
        Register m_reg;
    };

    class Move : public Instruction {
    public:
        Move(Register src, Register dst) : m_src(src), m_dst(dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Move) + sizeof(bytecode::Opcode);
        }

    private:
        Register m_src, m_dst;
    };

    class Unary : public Instruction {
    public:
        Unary(Register value, Register dst) : m_value(value), m_dst(dst) {}
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Not) + sizeof(bytecode::Opcode);
        }

    protected:
        Register m_value, m_dst;
    };

    class Not : public Unary {
    public:
        Not(Register value, Register dst) : Unary(value, dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Not) + sizeof(bytecode::Opcode);
        }
    };

    class Neg : public Unary {
    public:
        Neg(Register value, Register dst) : Unary(value, dst) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Neg) + sizeof(bytecode::Opcode);
        }
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
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Add) + sizeof(bytecode::Opcode);
        }
    };

    class Sub : public Binary {
    public:
        Sub(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Sub) + sizeof(bytecode::Opcode);
        }
    };

    class Mul : public Binary {
    public:
        Mul(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Mul) + sizeof(bytecode::Opcode);
        }
    };

    class Div : public Binary {
    public:
        Div(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Div) + sizeof(bytecode::Opcode);
        }
    };

    class Lt : public Binary {
    public:
        Lt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Lt) + sizeof(bytecode::Opcode);
        }
    };

    class Lte : public Binary {
    public:
        Lte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Lte) + sizeof(bytecode::Opcode);
        }
    };

    class Gt : public Binary {
    public:
        Gt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Gt) + sizeof(bytecode::Opcode);
        }
    };

    class Gte : public Binary {
    public:
        Gte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Gte) + sizeof(bytecode::Opcode);
        }
    };

    class Eq : public Binary {
    public:
        Eq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Eq) + sizeof(bytecode::Opcode);
        }
    };

    class Neq : public Binary {
    public:
        Neq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Neq) + sizeof(bytecode::Opcode);
        }
    };

    class Jmp : public BlockTerminator {
    public:
        Jmp(BasicBlock *target) : m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Jmp) + sizeof(bytecode::Opcode);
        }

    private:
        BasicBlock *m_target;
    };

    class JmpIfFalse : public BlockTerminator {
    public:
        JmpIfFalse(Register condition, BasicBlock *target) :
            m_condition(condition), m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;

        constexpr std::size_t length() const override {
            return sizeof(bytecode::JmpIfFalse) + sizeof(bytecode::Opcode);
        }

    private:
        Register m_condition;
        BasicBlock *m_target;
    };

    class JmpIfTrue : public BlockTerminator {
    public:
        JmpIfTrue(Register condition, BasicBlock *target) :
            m_condition(condition), m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::JmpIfTrue) + sizeof(bytecode::Opcode);
        }

    private:
        Register m_condition;
        BasicBlock *m_target;
    };

    class Branch : public BlockTerminator {
    public:
        Branch(Register condition, BasicBlock *true_target, BasicBlock *false_target) :
            m_condition(condition), m_true_target(true_target), m_false_target(false_target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::JmpIfTrue) + sizeof(bytecode::Jmp) +
                   sizeof(bytecode::Opcode) * 2;
        }

    private:
        Register m_condition;
        BasicBlock *m_true_target;
        BasicBlock *m_false_target;
    };


    class Call : public Instruction {
    public:
        Call(Register callee, Register result, size_t arg_count) :
            m_callee(callee), m_result(result), m_arg_count(arg_count) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Call) + sizeof(bytecode::Opcode);
        }

    private:
        Register m_callee;
        Register m_result;
        size_t m_arg_count;
    };


    class Ret : public BlockTerminator {
    public:
        Ret(Register value) : m_value(value) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Ret) + sizeof(bytecode::Opcode);
        }

    private:
        Register m_value;
    };

    class RetNil : public BlockTerminator {
    public:
        RetNil() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
    };
} // namespace Nyx::ir
