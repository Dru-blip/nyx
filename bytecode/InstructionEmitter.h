#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include "ir/Register.h"

namespace Nyx::bytecode {
    enum class Opcode : uint8_t;

    using Register = ir::Register;

    class InstructionEmitter {
    public:
        void push(const uint8_t byte) { m_code.push_back(byte); }

        template<typename InstType>
        void emit(const InstType &inst) {
            m_code.resize(m_code.size() + sizeof(InstType));
            std::memcpy(m_code.data() + m_code.size() - sizeof(InstType), &inst, sizeof(InstType));
        }

        template<typename InstType, typename... Args>
        void emit(Opcode opcode, Args... args) {
            push(static_cast<uint8_t>(opcode));
            InstType inst(args...);
            emit(inst);
        }


        std::vector<uint8_t> &code() { return m_code; }

        void ret(const Register &reg);
        void load_imm_int(const Register &reg, const int64_t &imm);
        void move(const Register &src, const Register &dst);

        void not_(const Register &arg, const Register &dst);
        void neg(const Register &arg, const Register &dst);

        void add(const Register &left, const Register &right, const Register &dst);
        void sub(const Register &left, const Register &right, const Register &dst);
        void mul(const Register &left, const Register &right, const Register &dst);
        void div(const Register &left, const Register &right, const Register &dst);
        void lt(const Register &left, const Register &right, const Register &dst);
        void lte(const Register &left, const Register &right, const Register &dst);
        void gt(const Register &left, const Register &right, const Register &dst);
        void gte(const Register &left, const Register &right, const Register &dst);
        void eq(const Register &left, const Register &right, const Register &dst);
        void neq(const Register &left, const Register &right, const Register &dst);

        void jmp(const uint16_t &offset);
        void jmp_if_false(const Register &arg, const uint32_t &offset);
        void jmp_if_true(const Register &arg, const uint32_t &offset);

    private:
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx::bytecode
