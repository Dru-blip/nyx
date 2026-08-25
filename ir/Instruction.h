#pragma once
#include <cstdint>
#include <vector>
#include "bytecode/Opcode.h"


namespace Nyx::ir {
    class BasicBlock;

    class Instruction {
    public:
        virtual ~Instruction() = default;
        virtual void lower(std::vector<uint8_t> &buffer) = 0;
        virtual bool is_terminator() const { return false; }

        // TODO: change the name , stack_cost is not accurate.
        virtual int stack_cost() const { return 0; }
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
            return sizeof(int64_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return 1; }
        void lower(std::vector<uint8_t> &buffer) override;

    private:
        int64_t m_value;
    };

    class LoadConst : public Instruction {
    public:
        LoadConst(uint16_t idx) : m_idx(idx) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint16_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return 1; }

    private:
        uint16_t m_idx;
    };

    class LoadString : public Instruction {
    public:
        LoadString(uint16_t idx) : m_idx(idx) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint16_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return 1; }

    private:
        uint16_t m_idx;
    };

    class StoreLocal : public Instruction {
    public:
        StoreLocal(uint8_t slot) : m_slot(slot) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint8_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return -1; }

    private:
        uint8_t m_slot;
    };


    class Unary : public Instruction {
    public:
        Unary() {}
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
        int stack_cost() const override { return 0; }

    protected:
    };

    class Not : public Unary {
    public:
        Not() {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
        int stack_cost() const override { return 0; }
    };

    class Neg : public Unary {
    public:
        Neg() {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
        int stack_cost() const override { return 0; }
    };

    class Binary : public Instruction {
    public:
        Binary() {}

        virtual constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
        int stack_cost() const override { return -1; }

    protected:
    };


    class Add : public Binary {
    public:
        Add() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Sub : public Binary {
    public:
        Sub() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Mul : public Binary {
    public:
        Mul() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Div : public Binary {
    public:
        Div() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Lt : public Binary {
    public:
        Lt() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Lte : public Binary {
    public:
        Lte() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Gt : public Binary {
    public:
        Gt() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Gte : public Binary {
    public:
        Gte() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Eq : public Binary {
    public:
        Eq() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Neq : public Binary {
    public:
        Neq() : Binary() {}
        void lower(std::vector<uint8_t> &buffer) override;
    };

    class Jmp : public BlockTerminator {
    public:
        Jmp(BasicBlock *target) : m_target(target) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint16_t) + sizeof(bytecode::Opcode);
        }

    private:
        BasicBlock *m_target;
    };

    class JmpIfFalse : public BlockTerminator {
    public:
        JmpIfFalse(BasicBlock *target) : m_target(target) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint16_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return -1; }

    private:
        BasicBlock *m_target;
    };

    class JmpIfTrue : public BlockTerminator {
    public:
        JmpIfTrue(BasicBlock *target) : m_target(target) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint16_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return -1; }

    private:
        BasicBlock *m_target;
    };

    class Branch : public BlockTerminator {
    public:
        Branch(BasicBlock *true_target, BasicBlock *false_target) :
            m_true_target(true_target), m_false_target(false_target) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return -1; }

    private:
        BasicBlock *m_true_target;
        BasicBlock *m_false_target;
    };


    class Call : public Instruction {
    public:
        Call(size_t arg_count) : m_arg_count(arg_count) {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override {
            return sizeof(uint8_t) + sizeof(bytecode::Opcode);
        }
        int stack_cost() const override { return (-m_arg_count - 1) + 1; }

    private:
        size_t m_arg_count;
    };


    class Ret : public BlockTerminator {
    public:
        Ret() {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
        int stack_cost() const override { return -1; }
    };

    class RetNil : public BlockTerminator {
    public:
        RetNil() {}
        void lower(std::vector<uint8_t> &buffer) override;
        constexpr std::size_t length() const override { return sizeof(bytecode::Opcode); }
    };
} // namespace Nyx::ir
