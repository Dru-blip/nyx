#pragma once

#include <cstdint>
#include <vector>
#include "heap/Cell.h"


namespace Nyx::bytecode {
    class Executable : public Cell {
    public:
        Executable(std::vector<uint8_t> &code) : m_code(std::move(code)) {}
        void print_code();

    private:
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx::bytecode
