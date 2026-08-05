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

        NodeRange to_node_range(std::uint32_t scratch_index);

        NodeIndex parse_stmt();
        NodeIndex parse_return_stmt();
        NodeIndex parse_expression(int8_t prec);
        NodeIndex parse_primary_expression();
        NodeIndex parse_integer();


        NodeIndex reserve_node(NodeTag tag);
        NodeIndex add_node(NodeTag tag, Span span);
        NodeIndex add_node(Node n);
        NodeIndex add_node(NodeTag tag, Span span, NodeRange range);
        void set_node(NodeIndex index, Node node);
        Span get_node_span(NodeIndex index);

        std::vector<std::uint32_t> m_scratch;
        std::vector<std::uint32_t> m_extra;
        std::vector<Node> m_nodes;
        const std::string_view m_source;
        Lexer m_lexer;
        Token m_cur, m_next;
    };
} // namespace Nyx
