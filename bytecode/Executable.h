#pragma once

#include <cstdint>
#include <vector>
#include "heap/Cell.h"


namespace Nyx::bytecode {
    class Executable : public Cell {

    private:
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx
