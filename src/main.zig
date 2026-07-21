const std = @import("std");
const Io = std.Io;

const cypher = @import("cypher");
const Ast = @import("./parser/Ast.zig");
const Heap = @import("heap/Heap.zig");

pub fn main(init: std.process.Init) !void {
    const arena: std.mem.Allocator = init.arena.allocator();

    var ast = try Ast.parse(arena, "return 5");
    ast.deinit();
    var heap = try Heap.init(arena);

    const cell = try heap.allocate(Ast);
    _ = cell;
}
