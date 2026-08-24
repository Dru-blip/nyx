#pragma once

#include <cstdint>
#include <cstring>
#include <vector>


namespace Nyx::bytecode {
    enum class Opcode : uint8_t;

    class InstructionEmitter {
    public:
        void push(const uint8_t byte) { m_code.push_back(byte); }

        template<typename InstType>
        inline void emit(const InstType &inst) {
            m_code.resize(m_code.size() + sizeof(InstType));
            std::memcpy(m_code.data() + m_code.size() - sizeof(InstType), &inst, sizeof(InstType));
        }

        template<typename InstType, typename... Args>
        inline void emit(Opcode opcode, Args... args) {
            push(static_cast<uint8_t>(opcode));
            InstType inst(args...);
            emit(inst);
        }


        std::vector<uint8_t> &code() { return m_code; }

        void ret();
        void ret_nil();

        void load_imm_int(const int64_t &imm);
        void load_const(const uint16_t &idx);
        void load_string(const uint16_t &idx);


        void not_();
        void neg();

        void add();
        void sub();
        void mul();
        void div();
        void lt();
        void lte();
        void gt();
        void gte();
        void eq();
        void neq();

        void call(const uint8_t &arg_count);
        void jmp(const uint16_t &offset);
        void jmp_if_false(const uint32_t &offset);
        void jmp_if_true(const uint32_t &offset);

    private:
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx::bytecode
