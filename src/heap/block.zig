const std = @import("std");

const region = @import("region.zig");

pub const BlockSize = 16 * 1024; // 16 KB

pub const Cell = struct {
    next: ?*Cell,
};

pub const Block = struct {
    base: [*]u8,
    gpa: std.mem.Allocator,
    region: *region.Region,
    cell_size: u16 = undefined,
    cell_count: u32 = undefined,
    free_list: ?*Cell,
    live: std.DynamicBitSet = undefined,
    next: ?*Block,

    pub fn init(gpa: std.mem.Allocator, base: [*]u8, reg: *region.Region) Block {
        return Block{
            .base = base,
            .gpa = gpa,
            .region = reg,
            .cell_count = 0,
            .free_list = null,
            .next = null,
        };
    }

    pub fn getFreeCell(self: *Block) ?*Cell {
        if (self.free_list) |cell| {
            self.free_list = cell.next;
            self.live.set(self.indexOf(cell));
            return cell;
        }
        return null;
    }

    pub inline fn indexOf(self: *Block, cell: *Cell) usize {
        return (@intFromPtr(cell) - @intFromPtr(self.base)) / self.cell_size;
    }

    pub fn format(self: *Block, class: u16) !void {
        self.cell_size = class;
        self.cell_count = BlockSize / class;

        self.live = try std.DynamicBitSet.initEmpty(self.gpa, self.cell_count);

        for (0..self.cell_count) |i| {
            const cell = @intFromPtr(self.base) + i * self.cell_size;
            const cell_ptr: *Cell = @ptrFromInt(cell);
            cell_ptr.next = self.free_list;
            self.free_list = cell_ptr;
        }
    }
};

pub const BlockAllocator = struct {
    regions: *region.RegionAllocator,
    free: ?*Block,

    pub fn init(regions: *region.RegionAllocator) BlockAllocator {
        return BlockAllocator{
            .regions = regions,
            .free = null,
        };
    }

    pub fn acquireBlock(self: *BlockAllocator) !?*Block {
        if (self.free == null) {
            try self.grow();
        }

        //IN: self.free is guaranteed to be non-null here
        const free = self.free.?;
        self.free = free.next;
        return free;
    }

    fn grow(self: *BlockAllocator) !void {
        const reg = try self.regions.acquireRegion();

        for (&reg.blocks) |*value| {
            value.next = self.free;
            self.free = value;
        }
    }
};
