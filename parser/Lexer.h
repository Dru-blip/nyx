#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"


namespace Nyx {
    class Lexer {
    public:
        Lexer(const std::string_view buffer) : m_source(buffer) {}
        std::vector<Token> tokenize();
        Token next_token();

    private:
        void skip_whitespaces();
        bool is_end() const;
        char current_char() const;
        char peek_char() const;
        void eat_integer();
        TokenTag eat_identifier();

        const std::string_view m_source;
        uint32_t m_pos = 0;

        // TODO: what happens if you remove inline.
        inline static const std::unordered_map<std::string_view, TokenTag> keyword_map{
                {"return", TokenTag::Return},
        };
    };
}; // namespace Nyx
