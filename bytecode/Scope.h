#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace Nyx::bytecode {
    enum class ScopeType {
        Module,
        Function,
    };

    struct Symbol {
        uint16_t slot;
        uint8_t depth;
        std::string_view name;
    };

    struct Scope {
        ScopeType type;
        std::vector<Symbol> symbols;
        uint8_t depth;


        void add_local(std::string_view name, uint16_t slot) {
            symbols.emplace_back(Symbol{slot, depth, name});
        }


        void begin_block() { depth++; }

        void end_block() {
            std::erase_if(symbols, [this](const Symbol &symbol) { return symbol.depth == depth; });
            depth--;
        }

        std::optional<uint16_t> resolve(const std::string_view name) {
            for (const auto &local: symbols) {
                if (local.name == name) {
                    return local.slot;
                }
            }

            return std::nullopt;
        }
    };
} // namespace Nyx::bytecode
