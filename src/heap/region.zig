const std = @import("std");

const block = @import("block.zig");

pub const RegionSize = 2 * 1024 * 1024; // 2 MB

pub const BlockCount = RegionSize / block.BlockSize;

// Bin(class sized container) -> BlockAllocator -> RegionAllocator
pub const Region = struct {
    raw: []u8,
    base: [*]u8,
    blocks: [BlockCount]block.Block,
    next: ?*Region,

    pub fn initBlocks(self: *Region, gpa: std.mem.Allocator) void {
        for (0..BlockCount) |i| {
            self.blocks[i] = block.Block.init(gpa, self.base + i * block.BlockSize, self);
        }
    }
};

pub const RegionAllocator = struct {
    head: ?*Region,
    gpa: std.mem.Allocator,
    arena: std.heap.ArenaAllocator,

    pub fn init(backing: std.mem.Allocator) RegionAllocator {
        return RegionAllocator{
            .head = null,
            .gpa = backing,
            .arena = std.heap.ArenaAllocator.init(backing),
        };
    }

    pub fn acquireRegion(self: *RegionAllocator) !*Region {
        const base = try std.heap.page_allocator.alloc(u8, RegionSize);
        const region = try self.gpa.create(Region);

        region.* = .{
            .base = base.ptr,
            .raw = base,
            .blocks = undefined,
            .next = self.head,
        };

        region.initBlocks(self.gpa);
        self.head = region;
        return region;
    }
};
