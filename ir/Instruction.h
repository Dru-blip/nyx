#pragma once
#include <cstdint>

#include "bytecode/Instruction.h"
#include "bytecode/InstructionEmitter.h"


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
        LoadImmInt(int64_t value) : m_value(value) {}
        constexpr std::size_t length() const override {
            return sizeof(bytecode::LoadImmInt) + sizeof(bytecode::Opcode);
        }
        void lower(InstructionEmitter &emitter) override;

    private:
        int64_t m_value;
    };

    class LoadConst : public Instruction {
    public:
        LoadConst(uint16_t idx) : m_idx(idx) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::LoadConst) + sizeof(bytecode::Opcode);
        }

    private:
        uint16_t m_idx;
    };

    class LoadString : public Instruction {
    public:
        LoadString(uint16_t idx) : m_idx(idx) {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::LoadString) + sizeof(bytecode::Opcode);
        }

    private:
        uint16_t m_idx;
    };


    class Unary : public Instruction {
    public:
        Unary() {}
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }

    protected:
    };

    class Not : public Unary {
    public:
        Not() {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
    };

    class Neg : public Unary {
    public:
        Neg() {}
        void lower(InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Neg) + sizeof(bytecode::Opcode);
        }
    };

    class Binary : public Instruction {
    public:
        Binary() {}

        virtual constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }

    protected:
    };


    class Add : public Binary {
    public:
        Add() : Binary() {}
        void lower(InstructionEmitter &emitter) override;
    };

    class Sub : public Binary {
    public:
        Sub() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Mul : public Binary {
    public:
        Mul() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Div : public Binary {
    public:
        Div() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Lt : public Binary {
    public:
        Lt() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Lte : public Binary {
    public:
        Lte() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Gt : public Binary {
    public:
        Gt() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Gte : public Binary {
    public:
        Gte() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Eq : public Binary {
    public:
        Eq() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Neq : public Binary {
    public:
        Neq() : Binary() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
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
        JmpIfFalse(BasicBlock *target) : m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::JmpIfFalse) + sizeof(bytecode::Opcode);
        }

    private:
        BasicBlock *m_target;
    };

    class JmpIfTrue : public BlockTerminator {
    public:
        JmpIfTrue(BasicBlock *target) : m_target(target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::JmpIfTrue) + sizeof(bytecode::Opcode);
        }

    private:
        BasicBlock *m_target;
    };

    class Branch : public BlockTerminator {
    public:
        Branch(BasicBlock *true_target, BasicBlock *false_target) :
            m_true_target(true_target), m_false_target(false_target) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::JmpIfTrue) + sizeof(bytecode::Jmp) +
                   sizeof(bytecode::Opcode) * 2;
        }

    private:
        BasicBlock *m_true_target;
        BasicBlock *m_false_target;
    };


    class Call : public Instruction {
    public:
        Call(size_t arg_count) : m_arg_count(arg_count) {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override {
            return sizeof(bytecode::Call) + sizeof(bytecode::Opcode);
        }

    private:
        size_t m_arg_count;
    };


    class Ret : public BlockTerminator {
    public:
        Ret() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
    };

    class RetNil : public BlockTerminator {
    public:
        RetNil() {}
        void lower(bytecode::InstructionEmitter &emitter) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
    };
} // namespace Nyx::ir
