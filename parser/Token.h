#pragma once
#include <algorithm>
#include <cstdint>


namespace Nyx {
    enum class TokenTag {
        Integer,
        String,
        Identifier,

        Plus,
        Minus,
        Asterisk,
        Slash,

        Bang,
        Tilde,

        Equal,
        EqualEqual,

        BangEqual,

        AngleBracketLeft,
        AngleBracketRight,
        AngleBracketLeftEqual,
        AngleBracketRightEqual,


        LeftParen,
        RightParen,
        LeftBrace,
        RightBrace,
        Semicolon,
        Comma,

        Return,
        Def,
        If,
        Else,
        Loop,
        Break,
        Continue,
        For,
        While,
        Var,
        And,
        Or,
        Not,
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
} // namespace Nyx
