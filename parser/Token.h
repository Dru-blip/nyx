#pragma once
#include <cstdint>
#include <algorithm>


namespace Nyx {
    enum class TokenTag {
        Number,
        Return,
        Invalid,
        Eof,
    };

    struct Span {
        uint32_t start;
        uint32_t end;

        Span merge(const Span &other) const {
            return Span(std::min(start, other.start), std::max(end, other.end));
        }
    };

    struct Token {
        TokenTag tag;
        Span span;
    };
}
