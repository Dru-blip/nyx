#include <cassert>
#include <memory>
#include "bytecode/Executable.h"
#include "bytecode/Generator.h"
#include "heap/Heap.h"
#include "parser/Ast.h"
#include "runtime/Fiber.h"

int main() {
    Nyx::Ast ast = Nyx::Ast::parse("return 5");
    std::shared_ptr<Nyx::Heap> heap = std::make_shared<Nyx::Heap>();

    Nyx::bytecode::Generator generator(ast, heap);
    Nyx::bytecode::Executable *executable = generator.compile();

    executable->print_code();

    Nyx::Fiber fiber;
    return 0;
}
