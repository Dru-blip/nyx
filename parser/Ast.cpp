#include "Ast.h"

#include "Parser.h"
#include "mimalloc.h"

namespace Nyx {
    NodeArena::NodeArena() { m_heap = mi_heap_new(); }

    NodeArena::~NodeArena() { mi_heap_destroy(m_heap); }

    Ast Ast::parse(const std::string_view source) {
        Parser parser(source);
        parser.parse();
        return parser.into_ast();
    }
} // namespace Nyx
