#pragma once
#include <optional>
#include <string_view>
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

    union NodeData {
        NodeIndex node_index{};
        std::optional<NodeIndex> opt_node;
        NodeRange range;
    };

    struct Node {
        NodeTag tag;
        Span span;
        NodeData data;

        //TODO: constructors for Node.
    };


    class Ast {
    public:
        Ast(std::vector<Node> &nodes, std::vector<uint32_t> &extras) : m_extras(std::move(extras)),
                                                                       m_nodes(std::move(nodes)) {
        };

        static Ast parse(std::string_view source);

        uint32_t length() const;

    private:
        std::vector<uint32_t> m_extras;
        std::vector<Node> m_nodes;
    };
}
