#pragma once

#include <cstdint>
#include <vector>
#include "heap/Cell.h"


namespace Nyx::bytecode {
    class Executable : public Cell {
    public:
        Executable(std::vector<uint8_t> &code) : m_code(std::move(code)) {}
        void print_code();
        size_t size() { return m_code.size(); }
        uint8_t *data() { return m_code.data(); }

    private:
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx::bytecode
