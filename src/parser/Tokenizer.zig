const Tokenizer = @This();

const std = @import("std");

pub const Span = struct {
    start: u32,
    end: u32,

    pub fn merge(a: Span, b: Span) Span {
        return .{
            .start = @min(a.start, b.start),
            .end = @max(a.end, b.end),
        };
    }
};

pub const Token = struct {
    tag: Tag,
    span: Span,

    pub const Tag = enum(u8) {
        integer,
        identifier,
        keyword_return,
        invalid,
        eof,
    };

    pub const List = std.ArrayList(Token);

    pub const keywords = std.StaticStringMap(Tag).initComptime(.{
        .{ "return", .keyword_return },
    });

    pub fn getKeyword(bytes: []const u8) ?Tag {
        return keywords.get(bytes);
    }
};

buffer: [:0]const u8,
pos: u32,

pub fn init(buffer: [:0]const u8) Tokenizer {
    return .{
        .buffer = buffer,
        .pos = 0,
    };
}

pub fn skip_whitespaces(self: *Tokenizer) void {
    while (std.ascii.isWhitespace(self.buffer[self.pos])) {
        self.pos += 1;
    }
}

pub fn read_integer(self: *Tokenizer) void {
    while (std.ascii.isDigit(self.buffer[self.pos])) {
        self.pos += 1;
    }
}

pub fn read_identifier(self: *Tokenizer) Token.Tag {
    const start: u32 = self.pos;
    while (std.ascii.isAlphanumeric(self.buffer[self.pos]) or self.buffer[self.pos] == '_') {
        self.pos += 1;
    }
    const bytes = self.buffer[start..self.pos];
    if (Token.getKeyword(bytes)) |tag| {
        return tag;
    }
    return .identifier;
}

pub fn next(self: *Tokenizer) Token {
    self.skip_whitespaces();
    var tag: Token.Tag = .invalid;
    const start: u32 = self.pos;

    const byte = self.buffer[self.pos];
    switch (byte) {
        0 => {
            tag = .eof;
            self.pos += 1;
        },
        '0'...'9' => {
            tag = .integer;
            self.read_integer();
        },
        'a'...'z', 'A'...'Z', '_' => {
            tag = self.read_identifier();
        },
        else => {
            tag = .invalid;
            self.pos += 1;
        },
    }

    return .{
        .tag = tag,
        .span = .{ .start = start, .end = self.pos },
    };
}

pub fn tokenize(gpa: std.mem.Allocator, buffer: [:0]const u8) !Token.List {
    var tokenizer = init(buffer);

    var tokens: Token.List = .empty;
    while (true) {
        const token = tokenizer.next();
        try tokens.append(gpa, token);
        if (token.tag == .eof) {
            break;
        }
    }
    return tokens;
}

test "keywords" {
    var tokens = try tokenize(std.testing.allocator, "return");
    defer tokens.deinit(std.testing.allocator);
    try std.testing.expect(tokens.items.len == 2);
    try std.testing.expect(tokens.items[0].tag == .keyword_return);
}

test "integer" {
    var tokens = try tokenize(std.testing.allocator, "42");
    defer tokens.deinit(std.testing.allocator);
    try std.testing.expect(tokens.items.len == 2);
    try std.testing.expect(tokens.items[0].tag == .integer);
}
