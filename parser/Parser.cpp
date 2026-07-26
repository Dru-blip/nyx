#include "Parser.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
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

    NodeIndex Parser::reserve_node(NodeTag tag) {
        const auto index = m_nodes.size();
        m_nodes.push_back(Node(tag));
        return index;
    }

    NodeIndex Parser::add_node(NodeTag tag, Span span) {
        const auto index = m_nodes.size();
        m_nodes.push_back(Node(tag, span));
        return index;
    }

    void Parser::set_node(NodeIndex index, Node node) { m_nodes[index] = node; }

    Span Parser::get_node_span(NodeIndex index) { return m_nodes[index].span; }

    Ast Parser::into_ast() { return {m_nodes, m_extra, m_source}; }

    NodeRange Parser::to_node_range(std::uint32_t scratch_index) {
        const auto start = m_extra.size();

        m_extra.insert(m_extra.end(), m_scratch.begin(), m_scratch.end());
        const auto end = m_extra.size();
        m_scratch.resize(scratch_index);

        return NodeRange(start, end);
    }

    void Parser::parse() {
        const auto root_index = reserve_node(NodeTag::Root);
        const auto scratch_index = m_scratch.size();

        while (!is_at_end()) {
            m_scratch.push_back(parse_stmt());
        }


        const auto node_range = to_node_range(scratch_index);
        const Node root(NodeTag::Root, Span(0, 0), node_range);
        set_node(root_index, root);
    }

    NodeIndex Parser::parse_stmt() {
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

    NodeIndex Parser::parse_return_stmt() {
        const auto ret_index = reserve_node(NodeTag::Ret);
        const auto ret_token = consume_token();

        NodeIndex value = parse_expression(0);

        const Span value_span = get_node_span(value);
        const Span ret_span = ret_token.span.merge(value_span);

        const Node n(NodeTag::Ret, ret_span, std::make_optional<NodeIndex>(value));
        set_node(ret_index, n);
        return ret_index;
    }

    NodeIndex Parser::parse_expression(int8_t prec) {
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

            const auto op_token = consume_token();
            const auto rhs = parse_expression(op_info.rbp);

            lhs = rhs;
        }

        return lhs;
    }

    NodeIndex Parser::parse_primary_expression() {
        const auto [tag, span] = m_cur;
        switch (tag) {
            case TokenTag::Integer: {
                return parse_integer();
            }
            default: {
                std::abort();
            }
        }
    }

    NodeIndex Parser::parse_integer() {
        const auto token = consume_token();
        return add_node(NodeTag::Integer, token.span);
    }
} // namespace Nyx
