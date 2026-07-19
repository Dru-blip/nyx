const Parser = @This();

const std = @import("std");

const Tokenizer = @import("Tokenizer.zig");
const Token = Tokenizer.Token;

const Ast = @import("Ast.zig");
const Node = Ast.Node;
const Span = Tokenizer.Span;

source: [:0]const u8,
tok: u32,
nodes: Node.List,
tokens: Token.List,
gpa: std.mem.Allocator,
extra: std.ArrayList(Node.Index),
scratch: std.ArrayList(Node.Index),

pub fn init(gpa: std.mem.Allocator, source: [:0]const u8, tokens: Token.List) Parser {
    return .{
        .source = source,
        .extra = .empty,
        .scratch = .empty,
        .nodes = .empty,
        .tok = 0,
        .gpa = gpa,
        .tokens = tokens,
    };
}

pub fn deinit(p: *Parser) void {
    p.scratch.deinit(p.gpa);
}

fn advance(p: *Parser) void {
    if (p.tok < p.tokens.items.len) {
        p.tok += 1;
    }
}

fn peek(p: *Parser) *const Token {
    return &p.tokens.items[p.tok];
}

fn eat(p: *Parser, expected: Token.Tag) ?Token {
    if (p.peek().tag != expected) return null;
    const token = p.peek();
    p.advance();
    return token.*;
}

fn atEnd(p: *Parser) bool {
    return p.tokens.items[p.tok].tag == .eof;
}

fn reserveNode(p: *Parser, tag: Node.Tag) !Node.Index {
    try p.nodes.resize(p.gpa, p.nodes.len + 1);
    const index = p.nodes.len;
    p.nodes.items(.tag)[index - 1] = tag;
    return @intCast(index - 1);
}

fn getNodeSpan(p: *Parser, index: Node.Index) Span {
    return p.nodes.items(.span)[index];
}

fn setNode(p: *Parser, index: Node.Index, node: Node) void {
    p.nodes.set(index, node);
}

fn addNode(p: *Parser, node: Node) !Node.Index {
    try p.nodes.append(p.gpa, node);
    return @intCast(p.nodes.len - 1);
}

fn scratchToExtra(p: *Parser, start: u32) !Node.Range {
    const range: Node.Range = .{
        .start = start,
        .end = @intCast(p.extra.items.len),
    };
    try p.extra.appendSlice(p.gpa, p.scratch.items[start..]);
    return range;
}

pub fn parse(p: *Parser) !void {
    const scratch_top = p.scratch.items.len;
    defer p.scratch.shrinkRetainingCapacity(scratch_top);

    const root = try p.reserveNode(.root);

    while (!p.atEnd()) {
        const stmt = try p.parseStatement();
        try p.scratch.append(p.gpa, stmt);
    }

    const range = try p.scratchToExtra(@intCast(scratch_top));

    p.setNode(root, .{
        .tag = .root,
        .span = .{ .start = 0, .end = 0 },
        .data = .{
            .range = range,
        },
    });
}

fn parseStatement(p: *Parser) !Node.Index {
    return p.parseReturn();
}

fn parseReturn(p: *Parser) !Node.Index {
    const ret_index = try p.reserveNode(.ret);
    const ret_tok = p.eat(.keyword_return).?;
    const value = try p.parseExpression();
    p.setNode(
        ret_index,
        .{
            .tag = .ret,
            .span = ret_tok.span.merge(p.getNodeSpan(value)),
            .data = .{
                .node = value,
            },
        },
    );
    return ret_index;
}

fn parseExpression(p: *Parser) !Node.Index {
    return p.parsePrimaryExpression();
}

fn parsePrimaryExpression(p: *Parser) !Node.Index {
    const int_tok = p.eat(.integer).?;

    return try p.addNode(.{
        .tag = .int,
        .span = int_tok.span,
        .data = .{
            .node = 10,
        },
    });
}
