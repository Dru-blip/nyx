#pragma once
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

#include "Token.h"


namespace Nyx {
    using NodeIndex = uint32_t;

    enum class NodeTag {
        Root,
        Number,
        Ret,
    };

    struct NodeRange {
        NodeIndex start;
        NodeIndex end;
    };

    constexpr NodeIndex INVALID_NODE_INDEX = std::numeric_limits<NodeIndex>::max();

    using NodeData = std::variant<NodeIndex, NodeRange>;

    struct Node {
        NodeTag tag;
        Span span{};
        NodeData data{INVALID_NODE_INDEX};


        Node(NodeTag tag) : tag(tag) {}
        Node(NodeTag tag, Span span) : tag(tag), span(span) {}
        Node(NodeTag tag, Span span, NodeIndex index) : tag(tag), span(span), data(index) {}
        Node(NodeTag tag, Span span, NodeRange range) : tag(tag), span(span), data(range) {}
        Node(NodeTag tag, Span span, std::optional<NodeIndex> opt) :
            tag(tag), span(span), data(opt.value_or(INVALID_NODE_INDEX)) {}
    };


    class Ast {
    public:
        Ast(std::vector<Node> &nodes, std::vector<uint32_t> &extras) :
            m_extras(std::move(extras)), m_nodes(std::move(nodes)) {};

        static Ast parse(std::string_view source);

        uint32_t length() const;

    private:
        std::vector<uint32_t> m_extras;
        std::vector<Node> m_nodes;
    };
} // namespace Nyx
