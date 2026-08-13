#include <cstdint>
#include "ir/Register.h"
// #include "bytecode/InstructionEmitter.h"

namespace Nyx::ir {
    class Instruction {
    public:
        virtual ~Instruction() = default;
        // virtual void lower(bytecode::InstructionEmitter &emitter) = 0;
    };

    class LoadImmInt : public Instruction {
    public:
        LoadImmInt(int64_t value, Register reg) : m_value(value), m_reg(reg) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;

    private:
        int64_t m_value;
        Register m_reg;
    };

    class Binary : public Instruction {
    public:
        Binary(Register lhs, Register rhs, Register dst) : m_lhs(lhs), m_rhs(rhs), m_dst(dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;

    private:
        Register m_lhs, m_rhs, m_dst;
    };


    class Add : public Binary {
    public:
        Add(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Sub : public Binary {
    public:
        Sub(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Mul : public Binary {
    public:
        Mul(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Div : public Binary {
    public:
        Div(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Lt : public Binary {
    public:
        Lt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Lte : public Binary {
    public:
        Lte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Gt : public Binary {
    public:
        Gt(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Gte : public Binary {
    public:
        Gte(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Eq : public Binary {
    public:
        Eq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };

    class Neq : public Binary {
    public:
        Neq(Register lhs, Register rhs, Register dst) : Binary(lhs, rhs, dst) {}
        // void lower(bytecode::InstructionEmitter &emitter) override;
    };


} // namespace Nyx::ir
