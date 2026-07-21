const std = @import("std");

const block = @import("block.zig");
const Block = block.Block;

// TODO: should reduce the max class sizes to below 4096
// and allocate large objects separately.
pub const Classes = [9]u16{ 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };

pub const Count = Classes.len;

pub fn getClassIndex(size: u16) usize {
    std.debug.assert(size > 0);
    const is_power_of_two = std.math.isPowerOfTwo(size);

    if (is_power_of_two and size >= Classes[0] and size <= Classes[Count - 1]) {
        return std.math.log2(size) - 5;
    }

    //TODO: handle sizes greater than the largest class
    // by throwing an error.
    if (size > Classes[Count - 1]) {
        //TODO: throw error.
        @panic("size too large to allocate");
    }

    // TODO: we have to check that size is within the range of the largest class.
    // otherwise, throw an error.

    const nearest = std.math.ceilPowerOfTwo(u16, size) catch {
        @panic("size overflow");
    };

    return std.math.log2(nearest) - 5;
}

test "getClassIndex" {
    const index = getClassIndex(128);
    try std.testing.expectEqual(2, index);
}

pub const Bin = struct {
    class: u16,
    active: ?*Block,
    used: ?*Block,
    block_allocator: *block.BlockAllocator,

    pub fn init(block_allocator: *block.BlockAllocator, bin_class: u16) Bin {
        return Bin{
            .class = bin_class,
            .active = null,
            .used = null,
            .block_allocator = block_allocator,
        };
    }

    pub fn acquireCell(self: *Bin, cell_size: u16) !*block.Cell {
        var cell: ?*block.Cell = null;
        if (self.active) |active| {
            cell = active.getFreeCell();
        }

        if (cell) |c| {
            return c;
        }

        const blk = try self.block_allocator.acquireBlock();
        try blk.?.format(cell_size);
        cell = blk.?.getFreeCell();

        if (self.active) |active| {
            active.next = self.used;
            self.used = active;
        }

        self.active = blk;
        return cell.?;
    }
};
