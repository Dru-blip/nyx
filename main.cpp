#include <cassert>
#include <memory>
#include "bytecode/Executable.h"
#include "bytecode/Generator.h"
#include "heap/Heap.h"
#include "parser/Ast.h"

int main() {
    Nyx::Ast ast = Nyx::Ast::parse("return 5");
    std::shared_ptr<Nyx::Heap> heap = std::make_shared<Nyx::Heap>();

    Nyx::bytecode::Generator generator(ast, heap);

    void *ptr = heap->allocate<Nyx::bytecode::Executable>();
    assert(ptr != nullptr);

    return 0;
}
