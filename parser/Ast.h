#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Token.h"


namespace Nyx {
    using NodeIndex = uint32_t;

    enum class NodeTag {
        Root,
        Integer,
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

        std::optional<NodeIndex> index() const {
            if (auto value = std::get_if<NodeIndex>(&data)) {
                if (*value != INVALID_NODE_INDEX) {
                    return *value;
                }
            }
            return std::nullopt;
        }
    };


    class Ast {
    public:
        Ast(std::vector<Node> &nodes, std::vector<uint32_t> &extras, std::string_view source) :
            source(source), m_extras(std::move(extras)), m_nodes(std::move(nodes)) {};

        static Ast parse(std::string_view source);

        uint32_t length() const;

        const Node &node(NodeIndex index) const { return m_nodes[index]; }
        std::string_view getSource(const Node &node) const {
            return std::string_view(source).substr(node.span.start,
                                                   node.span.end - node.span.start);
        }

    private:
        std::string_view source;
        std::vector<uint32_t> m_extras;
        std::vector<Node> m_nodes;
    };
} // namespace Nyx
