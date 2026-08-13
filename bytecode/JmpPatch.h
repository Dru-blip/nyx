#pragma once

#include <cstdint>
#include <vector>

namespace Nyx::bytecode {
    struct JmpPatch {
        uint16_t offset{0};
        uint16_t target{0};
        uint8_t result;


        template<typename Instr>
        void patch(std::vector<uint8_t> &code, const uint16_t &target) {
            Instr *instr = reinterpret_cast<Instr *>(code.data() + offset);
            instr->offset = target;
        };
    };
} // namespace Nyx::bytecode
