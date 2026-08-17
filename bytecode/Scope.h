#pragma once


#include <cstdint>
#include <string_view>
#include <vector>

namespace Nyx::bytecode {
    struct Symbol {
        uint8_t slot;
        std::string_view name;
    };

    struct Scope {
        std::vector<Symbol> locals;

        void add_local(std::string_view name, uint8_t slot) {
            locals.emplace_back(Symbol{slot, name});
        }
    };
} // namespace Nyx::bytecode
