#include <cassert>
#include "bytecode/Compiler.h"
#include "heap/Heap.h"
#include "parser/Ast.h"

int main() {
    Nyx::Ast ast = Nyx::Ast::parse("return 5");
    // Nyx::Compiler compiler(ast);
    // compiler.compile();

    Nyx::Heap *heap = Nyx::Heap::create();

    void *ptr = heap->allocate(128);
    assert(ptr != nullptr);


    return 0;
}
