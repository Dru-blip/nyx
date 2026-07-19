const std = @import("std");
const Io = std.Io;

const cypher = @import("cypher");
const Ast = @import("./parser/Ast.zig");

pub fn main(init: std.process.Init) !void {
    const arena: std.mem.Allocator = init.arena.allocator();
    _ = arena;
    var ast = try Ast.parse(init.gpa, "return 5");
    ast.deinit();
}
