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
        Executable(std::vector<uint8_t> &code, std::vector<Value> &constants, size_t stack_size) :
            m_code(std::move(code)), m_constants(std::move(constants)), m_stack_size(stack_size) {}


        void print_code();
        Value *constants() { return m_constants.data(); }
        size_t size() { return m_code.size(); }
        uint8_t *data() { return m_code.data(); }
        size_t stack_size() { return m_stack_size; }


    private:
        template<typename Instr>
        void print_binary_instruction(std::string_view name, std::string_view op, uint8_t *&pc) {
            Instr instr;
            std::memcpy(&instr, pc, sizeof(Instr));
            std::println("[{}] {} %{}", pc - m_code.data(), name, op);
            pc += sizeof(Instr);
        }

        template<typename Instr>
        void print_unary_instruction(std::string_view name, std::string_view op, uint8_t *&pc) {
            Instr instr;
            std::memcpy(&instr, pc, sizeof(Instr));
            std::println("[{}] {} %{}", pc - m_code.data(), name, op);
            pc += sizeof(Instr);
        }

        std::vector<uint8_t> m_code;
        std::vector<Value> m_constants;
        size_t m_stack_size{0};
    };
} // namespace Nyx::bytecode
