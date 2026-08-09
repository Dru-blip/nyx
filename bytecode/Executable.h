#pragma once

#include <cstdint>
#include <vector>
#include "heap/Cell.h"
#include "runtime/Value.h"


namespace Nyx::bytecode {
    class Executable : public Cell {
    public:
        Executable(std::vector<uint8_t> &code, std::vector<Value> &constants) :
            m_code(std::move(code)), m_constants(std::move(constants)) {}
        void print_code();
        size_t size() { return m_code.size(); }
        uint8_t *data() { return m_code.data(); }
        uint8_t register_count() { return m_register_count; }


    private:
        std::vector<uint8_t> m_code;
        std::vector<Value> m_constants;
        uint8_t m_register_count{0};
    };
} // namespace Nyx::bytecode
