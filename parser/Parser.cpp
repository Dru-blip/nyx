#include "Parser.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <optional>
#include <print>
#include "parser/Ast.h"
#include "parser/Token.h"

struct Operator {
    Nyx::NodeTag tag;
    int8_t lbp;
    int8_t rbp;
};

namespace Nyx {
    static std::unordered_map<TokenTag, Operator> operator_map{
            {TokenTag::Equal, {NodeTag::Eq, 30, 31}},
            {TokenTag::BangEqual, {NodeTag::Neq, 30, 31}},

            {TokenTag::AngleBracketLeft, {NodeTag::Lt, 40, 41}},
            {TokenTag::AngleBracketRight, {NodeTag::Gt, 40, 41}},
            {TokenTag::AngleBracketLeftEqual, {NodeTag::Gte, 40, 41}},
            {TokenTag::AngleBracketRightEqual, {NodeTag::Lte, 40, 41}},


            {TokenTag::Plus, {NodeTag::Add, 50, 51}},
            {TokenTag::Minus, {NodeTag::Sub, 50, 51}},
            {TokenTag::Asterisk, {NodeTag::Mul, 52, 53}},
            {TokenTag::Slash, {NodeTag::Div, 52, 53}},
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
            case TokenTag::Return: {
                return parse_return_stmt();
            }
            default: {
                std::abort();
            }
        }
    }

    Node *Parser::parse_return_stmt() {
        const auto ret_token = consume_token();
        Node *value = parse_expression(0);
        const Span ret_span = ret_token.span.merge(value->span);
        Node *ret = m_arena.allocate<Return>(ret_span, std::make_optional(value));
        return ret;
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
            const auto rhs = parse_expression(op_info.rbp);

            const Span span = lhs->span.merge(rhs->span);
            Node *n = m_arena.allocate<Binary>(op_info.tag, span, lhs, rhs);
            lhs = n;
        }

        return lhs;
    }

    Node *Parser::parse_primary_expression() {
        const auto [tag, span] = m_cur;
        switch (tag) {
            case TokenTag::Integer: {
                return parse_integer();
            }
            case TokenTag::Minus: {
                const auto _ = consume_token();
                const auto rhs = parse_primary_expression();
                return m_arena.allocate<Neg>(span.merge(rhs->span), rhs);
            }
            case TokenTag::Bang: {
                const auto _ = consume_token();
                const auto rhs = parse_primary_expression();
                return m_arena.allocate<Not>(span.merge(rhs->span), rhs);
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
