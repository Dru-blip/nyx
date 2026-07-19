const Ast = @This();
const std = @import("std");

const Parser = @import("Parser.zig");
const Tokenizer = @import("Tokenizer.zig");

const Span = Tokenizer.Span;

pub const Node = struct {
    tag: Tag,
    span: Span,
    data: Data,

    pub const Tag = enum(u8) {
        root,
        int,
        ret,
    };

    pub const Index = u32;

    pub const Data = union {
        node: Index,
        opt: ?Index,
        range: Range,
    };

    pub const Range = struct {
        start: Index,
        end: Index,
    };

    pub const List = std.MultiArrayList(Node);
};

source: [:0]const u8,
extra: []u32,
nodes: Node.List.Slice,
gpa: std.mem.Allocator,

pub fn parse(gpa: std.mem.Allocator, source: [:0]const u8) !Ast {
    var tokens = try Tokenizer.tokenize(gpa, source);
    var parser = Parser.init(gpa, source, tokens);
    defer tokens.deinit(gpa);
    defer parser.deinit();
    try parser.parse();
    return .{
        .nodes = parser.nodes.toOwnedSlice(),
        .extra = try parser.extra.toOwnedSlice(gpa),
        .source = source,
        .gpa = gpa,
    };
}

pub fn deinit(self: *Ast) void {
    self.gpa.free(self.extra);
    self.nodes.deinit(self.gpa);
}
