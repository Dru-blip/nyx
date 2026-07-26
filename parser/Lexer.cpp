#include "Lexer.h"


namespace Nyx {
    bool Lexer::is_end() const { return m_pos >= m_source.length(); }

    void Lexer::skip_whitespaces() {
        while (!is_end() && std::isspace(m_source[m_pos])) {
            m_pos++;
        }
    }

    char Lexer::current_char() const { return is_end() ? '\0' : m_source[m_pos]; }

    void Lexer::eat_integer() {
        while (!is_end() && std::isdigit(m_source[m_pos])) {
            m_pos++;
        }
    }

    TokenTag Lexer::eat_identifier() {
        const auto start = m_pos;
        while (!is_end() && std::isalnum(m_source[m_pos])) {
            m_pos++;
        }
        const auto end = m_pos;
        const auto str = m_source.substr(start, end - start);
        if (const auto it = keyword_map.find(str); it != keyword_map.end()) {
            return it->second;
        }
        return TokenTag::Invalid;
    }

    Token Lexer::next_token() {
        skip_whitespaces();

        char c = current_char();
        const auto start = m_pos;
        auto tag = TokenTag::Invalid;

        switch (c) {
            case '\0': {
                tag = TokenTag::Eof;
                m_pos++;
                break;
            }
            case '+':
                tag = TokenTag::Plus;
                m_pos++;
                break;
            case '-':
                tag = TokenTag::Minus;
                m_pos++;
                break;
            case '*':
                tag = TokenTag::Asterisk;
                m_pos++;
                break;
            case '/':
                tag = TokenTag::Slash;
                m_pos++;
                break;
            default: {
                if (std::isdigit(c)) {
                    tag = TokenTag::Integer;
                    eat_integer();
                    break;
                }
                if (std::isalpha(c)) {
                    tag = eat_identifier();
                }

                break;
            }
        }

        return Token(tag, Span(start, m_pos));
    }

    std::vector<Token> Lexer::tokenize() {
        auto tokens = std::vector<Token>();
        while (true) {
            const Token token = next_token();
            tokens.push_back(token);
            if (token.tag == TokenTag::Eof) {
                break;
            }
        }
        return tokens;
    }
} // namespace Nyx
