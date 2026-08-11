#include <cassert>
#include <cstdio>
#include <memory>
#include "bytecode/Executable.h"
#include "bytecode/Generator.h"
#include "heap/Heap.h"
#include "parser/Ast.h"
#include "runtime/Fiber.h"

int main() {
    Nyx::Ast ast = Nyx::Ast::parse("return -!2");
    std::shared_ptr<Nyx::Heap> heap = std::make_shared<Nyx::Heap>();

    Nyx::bytecode::Generator generator(ast, heap);
    Nyx::bytecode::Executable *executable = generator.compile();

    executable->print_code();

    // Nyx::Fiber fiber;
    // Nyx::Value result = fiber.run(executable);

    // std::printf("%lld\n", result.as_int());

    return 0;
}
