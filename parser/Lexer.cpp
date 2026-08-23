#include "Lexer.h"
#include <cctype>


namespace Nyx {
    bool Lexer::is_end() const { return m_pos >= m_source.length(); }

    void Lexer::skip_whitespaces() {
        while (!is_end() && std::isspace(m_source[m_pos])) {
            m_pos++;
        }
    }

    char Lexer::current_char() const { return is_end() ? '\0' : m_source[m_pos]; }
    char Lexer::peek_char() const { return is_end() ? '\0' : m_source[m_pos + 1]; }

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
        return TokenTag::Identifier;
    }

    void Lexer::eat_string() {
        // TODO: handle escaped sequences.
        m_pos++;
        while (!is_end() && current_char() != '"') {
            m_pos++;
        }

        if (current_char() == '"') {
            m_pos++;
        }

        // TODO: handle non terminated strings.
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
            case '(': {
                tag = TokenTag::LeftParen;
                m_pos++;
                break;
            }
            case ';': {
                tag = TokenTag::Semicolon;
                m_pos++;
                break;
            }
            case ')': {
                tag = TokenTag::RightParen;
                m_pos++;
                break;
            }
            case '{': {
                tag = TokenTag::LeftBrace;
                m_pos++;
                break;
            }
            case '}': {
                tag = TokenTag::RightBrace;
                m_pos++;
                break;
            }
            case ',': {
                tag = TokenTag::Comma;
                m_pos++;
                break;
            }
            case '!': {
                if (peek_char() == '=') {
                    tag = TokenTag::BangEqual;
                    m_pos += 2;
                    break;
                }
                tag = TokenTag::Bang;
                m_pos++;
                break;
            }
            case '~': {
                tag = TokenTag::Tilde;
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
            case '<': {
                if (peek_char() == '=') {
                    tag = TokenTag::AngleBracketLeftEqual;
                    m_pos += 2;
                    break;
                }
                tag = TokenTag::AngleBracketLeft;
                m_pos++;
                break;
            }
            case '>': {
                if (peek_char() == '=') {
                    tag = TokenTag::AngleBracketRightEqual;
                    m_pos += 2;
                    break;
                }
                tag = TokenTag::AngleBracketRight;
                m_pos++;
                break;
            }
            case '=': {
                if (peek_char() == '=') {
                    tag = TokenTag::EqualEqual;
                    m_pos += 2;
                    break;
                }
                tag = TokenTag::Equal;
                m_pos++;
                break;
            }
            case '"': {
                tag = TokenTag::String;
                eat_string();
                break;
            }
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
