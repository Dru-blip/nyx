#pragma once
#include <cstdint>
#include "Ast.h"
#include "Lexer.h"


namespace Nyx {
    class Parser {
    public:
        Parser(std::string_view source);
        void parse();
        Ast into_ast();

    private:
        void advance();
        bool is_at_end() const;

        Token consume_token();


        Node *parse_stmt();
        Node *parse_return_stmt();
        Node *parse_expression(int8_t prec);
        Node *parse_primary_expression();
        Node *parse_integer();


        NodeArena m_arena;
        std::span<Node *> m_roots;
        const std::string_view m_source;
        Lexer m_lexer;
        Token m_cur, m_next;
    };
} // namespace Nyx
