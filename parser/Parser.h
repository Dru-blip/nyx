#pragma once
#include <cstdint>
#include <span>
#include "Ast.h"
#include "Lexer.h"
#include "parser/Token.h"


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
        Token expect_token(TokenTag tag);


        Node *parse_stmt();
        Node *parse_fn_decl();
        std::span<Node*> parse_fn_params();
        Node *parse_var_decl();
        Node *parse_while_loop();
        Node *parse_loop_stmt();
        Node *parse_break_stmt();
        Node *parse_continue_stmt();
        Node *parse_if_stmt();
        Node *parse_block_stmt();

        Node *parse_return_stmt();
        Node *parse_expr_stmt();

        Node *parse_expression(int8_t prec);
        Node *parse_postfix_expression(Node *lhs, NodeTag tag);
        Node *parse_primary_expression();
        Node *parse_integer();


        NodeArena m_arena;
        std::span<Node *> m_roots;
        const std::string_view m_source;
        Lexer m_lexer;
        Token m_cur, m_next;
    };
} // namespace Nyx
