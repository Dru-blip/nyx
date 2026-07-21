#include "Parser.h"

namespace Nyx {
    Parser::Parser(const std::string_view source) : m_source(source), m_lexer(Lexer(source)) {
        advance();
        advance();
    }

    void Parser::advance() {
        m_cur = m_next;
        m_next = m_lexer.next_token();
    }

    bool Parser::is_at_end() const {
        return m_cur.tag == TokenTag::Eof;
    }

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

    void Parser::set_node(NodeIndex index, Node node) {
        m_nodes[index] = node;
    }

    Span Parser::get_node_span(NodeIndex index) {
        return m_nodes[index].span;
    }

    Ast Parser::into_ast() {
        return {m_nodes, m_extra};
    }

    void Parser::parse() {
        const auto root_index = reserve_node(NodeTag::Root);
        const auto scratch_index = m_scratch.size();
        while (!is_at_end()) {
            m_scratch.push_back(parse_stmt());
        }


        const auto extra_index = m_extra.size();
        m_extra.resize(m_extra.size() + m_scratch.size());
        m_extra.insert(m_extra.end(), m_scratch.begin(), m_scratch.end());

        const Node root(NodeTag::Root, {
                            .start = 0,
                            .end = 0,
                        }, {
                            .range = NodeRange(extra_index, extra_index + m_scratch.size()),
                        });
        m_scratch.resize(scratch_index);
        set_node(root_index, root);
    }

    NodeIndex Parser::parse_stmt() {
        const auto [tag,span] = m_cur;
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

        NodeIndex value = parse_expression();

        const Span value_span = get_node_span(value);
        const Span ret_span = ret_token.span.merge(value_span);

        const Node n(NodeTag::Ret, ret_span, {
                         .opt_node = std::make_optional<NodeIndex>(value),
                     });

        set_node(ret_index, n);

        return ret_index;
    }

    NodeIndex Parser::parse_expression() {
        return parse_number();
    }

    NodeIndex Parser::parse_number() {
        const auto token = consume_token();
        return add_node(NodeTag::Number, token.span);
    }
}
