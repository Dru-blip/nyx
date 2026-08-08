#include "Ast.h"
#include "Parser.h"
#include "mimalloc.h"

namespace Nyx {
    NodeArena::NodeArena() {
        // TODO: handle error.
        m_heap = mi_heap_new();
    }

    NodeArena::~NodeArena() {
        if (m_heap) {
            mi_heap_destroy(m_heap);
            m_heap = nullptr;
        }
    }

    Ast Ast::parse(const std::string_view source) {
        Parser parser(source);
        parser.parse();
        return parser.into_ast();
    }
} // namespace Nyx
