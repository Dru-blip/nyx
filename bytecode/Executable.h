#pragma once

#include <cstdint>
#include <cstring>
#include <print>
#include <string_view>
#include <vector>
#include "heap/Cell.h"
#include "runtime/Value.h"


namespace Nyx::bytecode {
    class Executable : public Cell {
    public:
        Executable(std::vector<uint8_t> &code, const uint8_t register_count,
                   std::vector<Value> &constants) :
            m_code(std::move(code)), m_constants(std::move(constants)),
            m_register_count(register_count) {}


        void print_code();
        Value *constants() { return m_constants.data(); }
        size_t size() { return m_code.size(); }
        uint8_t *data() { return m_code.data(); }
        uint8_t register_count() { return m_register_count; }


    private:
        template<typename Instr>
        void print_binary_instruction(std::string_view name, std::string_view op, uint8_t *&pc) {
            Instr instr;
            std::memcpy(&instr, pc, sizeof(Instr));
            std::println("[{}] {} %{} = %{} {} %{}", pc - m_code.data(), name, instr.reg, instr.lhs,
                         op, instr.rhs);
            pc += sizeof(Instr);
        }

        template<typename Instr>
        void print_unary_instruction(std::string_view name, std::string_view op, uint8_t *&pc) {
            Instr instr;
            std::memcpy(&instr, pc, sizeof(Instr));
            std::println("[{}] {} %{} = {}%{}", pc - m_code.data(), name, instr.arg, op,
                         instr.result);
            pc += sizeof(Instr);
        }

        std::vector<uint8_t> m_code;
        std::vector<Value> m_constants;
        uint8_t m_register_count{0};
    };
} // namespace Nyx::bytecode
