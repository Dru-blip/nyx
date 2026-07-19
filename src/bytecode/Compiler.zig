const Compiler = @This();

const std = @import("std");
const Ast = @import("../parser/Ast.zig");
const Opcode = @import("./opcodes.zig").Opcode;

const InstructionStream = std.ArrayList(u8);

code: InstructionStream = .empty,
gpa: std.mem.Allocator,

ast: Ast,

pub fn init(gpa: std.mem.Allocator, ast: Ast) Compiler {
    return .{
        .gpa = gpa,
        .ast = ast,
    };
}

fn compile(self: *Compiler) !void {
    const roots = self.ast.nodes.get(0).data.range;

    for (roots.start..roots.end) |index| {
        const root = self.ast.nodes.get(index);
        try self.lowerRoot(root);
    }
}

fn lowerRoot(self: *Compiler, node: Ast.Node) !void {
    switch (node.tag) {
        .ret => {
            try self.lowerRet(node);
        },
        _ => unreachable,
    }
}

fn lowerRet(self: *Compiler, node: Ast.Node) !void {
    if (node.data.opt) |val_index| {
        try self.lowerExpr(val_index);
    }
    try self.code.append(self.gpa, Opcode.to(.ret));
}

fn lowerExpr(self: *Compiler, node: Ast.Node) !void {
    switch (node.tag) {
        .int => {
            try self.lowerInt(node);
        },
        _ => unreachable,
    }
}

fn lowerInt(self: *Compiler, node: Ast.Node) !void {
    try self.emit(Opcode.to(.load_imm_int));

    const value = self.ast.getSource(node);

    //TODO: handle parsing error.
    const int_value = std.fmt.parseInt(i64, value, 10) catch 0;
    const aw: std.Io.Writer.Allocating = .fromArrayList(self.gpa, &self.code);
    defer aw.deinit();
    try aw.writer.writeInt(i64, int_value, .little);
}

fn emit(self: *Compiler, byte: u8) !void {
    try self.code.append(self.gpa, byte);
}
