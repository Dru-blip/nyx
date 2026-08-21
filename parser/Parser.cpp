#include "Parser.h"
#include <cstdint>
#include <optional>
#include <print>
#include <span>
#include <vector>
#include "parser/Ast.h"
#include "parser/Token.h"


struct Operator {
    Nyx::NodeTag tag;
    int8_t lbp;
    int8_t rbp;
    bool is_postfix{false};
};


namespace Nyx {
    static std::unordered_map<TokenTag, Operator> operator_map{
            {TokenTag::Equal, {NodeTag::Assignment, 1, 1}},

            {TokenTag::Or, {NodeTag::Or, 16, 17}},
            {TokenTag::And, {NodeTag::And, 17, 18}},

            {TokenTag::EqualEqual, {NodeTag::Eq, 30, 31}},
            {TokenTag::BangEqual, {NodeTag::Neq, 30, 31}},

            {TokenTag::AngleBracketLeft, {NodeTag::Lt, 40, 41}},
            {TokenTag::AngleBracketRight, {NodeTag::Gt, 40, 41}},
            {TokenTag::AngleBracketRightEqual, {NodeTag::Gte, 40, 41}},
            {TokenTag::AngleBracketLeftEqual, {NodeTag::Lte, 40, 41}},


            {TokenTag::Plus, {NodeTag::Add, 50, 51}},
            {TokenTag::Minus, {NodeTag::Sub, 50, 51}},
            {TokenTag::Asterisk, {NodeTag::Mul, 52, 53}},
            {TokenTag::Slash, {NodeTag::Div, 52, 53}},

            {TokenTag::LeftParen, {NodeTag::Call, 100, 101, true}},
    };

    Parser::Parser(const std::string_view source) : m_source(source), m_lexer(Lexer(source)) {
        advance();
        advance();
    }

    void Parser::advance() {
        m_cur = m_next;
        m_next = m_lexer.next_token();
    }

    bool Parser::is_at_end() const { return m_cur.tag == TokenTag::Eof; }

    Token Parser::consume_token() {
        const auto tok = m_cur;
        advance();
        return tok;
    }

    Token Parser::expect_token(TokenTag tag) {
        if (m_cur.tag != tag) {
            std::println("token {} expected, got {}", static_cast<int>(tag),
                         static_cast<int>(m_cur.tag));
            // TODO: raise an error if token mismatch.
            abort();
        }
        return consume_token();
    }

    Ast Parser::into_ast() { return {m_source, m_arena, m_roots}; }

    void Parser::parse() {
        std::vector<Node *> stmts;

        while (!is_at_end()) {
            stmts.push_back(parse_stmt());
        }

        m_roots = m_arena.nodes_span(stmts);
    }

    Node *Parser::parse_stmt() {
        const auto [tag, span] = m_cur;
        switch (tag) {
            case TokenTag::Var: {
                return parse_var_decl();
            }
            case TokenTag::While: {
                return parse_while_loop();
            }
            case TokenTag::Loop: {
                return parse_loop_stmt();
            }
            case TokenTag::Break: {
                return parse_break_stmt();
            }
            case TokenTag::Continue: {
                return parse_continue_stmt();
            }
            case TokenTag::If: {
                return parse_if_stmt();
            }
            case TokenTag::LeftBrace: {
                return parse_block_stmt();
            }
            case TokenTag::Return: {
                return parse_return_stmt();
            }
            default: {
                return parse_expr_stmt();
            }
        }
    }

    Node *Parser::parse_var_decl() {
        const auto var_token = consume_token();

        const auto name = expect_token(TokenTag::Identifier);
        const auto _ = expect_token(TokenTag::Equal);
        const auto initializer = parse_expression(0);
        const auto semi_token = expect_token(TokenTag::Semicolon);

        return m_arena.allocate<VarDecl>(var_token.span.merge(semi_token.span), name.span,
                                         initializer);
    }

    Node *Parser::parse_while_loop() {
        const auto while_token = consume_token();
        const auto test = parse_expression(0);
        const auto body = parse_stmt();
        return m_arena.allocate<WhileLoop>(while_token.span.merge(body->span), test, body);
    }

    Node *Parser::parse_loop_stmt() {
        const auto loop_token = consume_token();
        const auto body = parse_stmt();
        return m_arena.allocate<Loop>(loop_token.span.merge(body->span), body);
    }

    Node *Parser::parse_break_stmt() {
        const auto break_token = consume_token();
        expect_token(TokenTag::Semicolon);
        return m_arena.allocate<Break>(break_token.span);
    }

    Node *Parser::parse_continue_stmt() {
        const auto continue_token = consume_token();
        expect_token(TokenTag::Semicolon);
        return m_arena.allocate<Continue>(continue_token.span);
    }

    Node *Parser::parse_if_stmt() {
        const auto if_token = consume_token();
        const auto test = parse_expression(0);
        const auto consequent = parse_stmt();
        Node *alternate{nullptr};
        if (m_cur.tag == TokenTag::Else) {
            consume_token();
            alternate = parse_stmt();
        }
        const Span span = alternate != nullptr ? if_token.span.merge(alternate->span)
                                               : if_token.span.merge(consequent->span);
        return m_arena.allocate<If>(span, test, consequent, alternate);
    }

    Node *Parser::parse_block_stmt() {
        const auto left_brace_token = consume_token();
        std::vector<Node *> stmts;

        while (m_cur.tag != TokenTag::RightBrace) {
            stmts.push_back(parse_stmt());
        }

        const auto right_brace_token = expect_token(TokenTag::RightBrace);
        const Span block_span = left_brace_token.span.merge(right_brace_token.span);
        std::span<Node *> stmts_span = m_arena.nodes_span(stmts);
        return m_arena.allocate<BlockStmt>(block_span, stmts_span);
    }

    Node *Parser::parse_return_stmt() {
        const auto ret_token = consume_token();
        Node *value = parse_expression(0);
        const auto semi_token = expect_token(TokenTag::Semicolon);
        const Span ret_span = ret_token.span.merge(semi_token.span);
        Node *ret = m_arena.allocate<Return>(ret_span, std::make_optional(value));
        return ret;
    }

    Node *Parser::parse_expr_stmt() {
        Node *expr = parse_expression(0);
        const auto semi_token = expect_token(TokenTag::Semicolon);
        const Span span = expr->span.merge(semi_token.span);
        return m_arena.allocate<ExprStmt>(span, expr);
    }

    Node *Parser::parse_expression(int8_t prec) {
        auto lhs = parse_primary_expression();

        while (true) {
            const auto tok = m_cur;
            if (!operator_map.contains(tok.tag)) {
                break;
            }

            Operator op_info = operator_map[tok.tag];
            if (op_info.lbp < prec) {
                break;
            }

            const auto _ = consume_token();

            if (op_info.is_postfix) {
                lhs = parse_postfix_expression(lhs, op_info.tag);
                continue;
            }
            const auto rhs = parse_expression(op_info.rbp);

            const Span span = lhs->span.merge(rhs->span);
            Node *n = m_arena.allocate<Binary>(op_info.tag, span, lhs, rhs);
            lhs = n;
        }

        return lhs;
    }

    Node *Parser::parse_postfix_expression(Node *lhs, NodeTag tag) {
        switch (tag) {
            case NodeTag::Call: {
                std::vector<Node *> args;
                while (m_cur.tag != TokenTag::RightParen) {
                    args.push_back(parse_expression(0));
                }
                const auto closing_paren = expect_token(TokenTag::RightParen);
                const std::span<Node *> args_span = m_arena.nodes_span(args);
                return m_arena.allocate<Call>(lhs->span.merge(closing_paren.span), lhs, args_span);
            }
            default: {
                // TODO: handle other postfix expressions
                abort();
            }
        }
    }

    Node *Parser::parse_primary_expression() {
        const auto [tag, span] = m_cur;
        switch (tag) {
            case TokenTag::Integer: {
                return parse_integer();
            }
            case TokenTag::String: {
                const auto _ = consume_token();
                return m_arena.allocate<StringLiteral>(span);
            }
            case TokenTag::Identifier: {
                const auto _ = consume_token();
                return m_arena.allocate<Identifier>(span);
            }
            case TokenTag::LeftParen: {
                const auto _ = consume_token();
                const auto expr = parse_expression(0);
                // TODO: check right_paren is TokenTag::RightParen
                const auto right_paren = consume_token();
                return m_arena.allocate<Unary>(NodeTag::Grouped, span.merge(right_paren.span),
                                               expr);
            }
            case TokenTag::Minus: {
                const auto _ = consume_token();
                const auto rhs = parse_primary_expression();
                return m_arena.allocate<Unary>(NodeTag::Neg, span.merge(rhs->span), rhs);
            }
            case TokenTag::Bang: {
                const auto _ = consume_token();
                const auto rhs = parse_primary_expression();
                return m_arena.allocate<Unary>(NodeTag::Not, span.merge(rhs->span), rhs);
            }
            default: {
                std::abort();
            }
        }
    }

    Node *Parser::parse_integer() {
        const auto token = consume_token();
        return m_arena.allocate<IntLiteral>(token.span);
    }
} // namespace Nyx
