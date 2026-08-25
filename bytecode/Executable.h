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
        Executable(std::vector<uint8_t> &code, std::vector<Value> &constants, uint32_t nlocals,
                   uint32_t stack_size) :
            m_code(std::move(code)), m_constants(std::move(constants)), m_nlocals(nlocals),
            m_stack_size(stack_size) {}


        void print_code();
        Value *constants() { return m_constants.data(); }
        size_t size() { return m_code.size(); }
        uint8_t *data() { return m_code.data(); }
        size_t stack_size() { return m_stack_size; }

        uint8_t num_locals() const { return m_nlocals; }
        uint32_t stack_size() const { return m_stack_size; }


    private:
        std::vector<uint8_t> m_code;
        std::vector<Value> m_constants;
        uint32_t m_nlocals;
        uint32_t m_stack_size;
    };
} // namespace Nyx::bytecode
