#pragma once

#include <cstdint>
#include <vector>


namespace Nyx::bytecode {
    class InstructionEmitter {
    public:
        template<typename InstType>
        void emit(InstType &inst) {
            const uint8_t *start = reinterpret_cast<uint8_t *>(&inst);
            const uint8_t *end = start + sizeof(InstType);

            m_code.insert(m_code.end(), start, end);
        }


        std::vector<uint8_t> &code() { return m_code; }
        void emit_ret();
        void emit_ret_imm(int64_t imm);

    private:
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx::bytecode
