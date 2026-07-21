const Heap = @This();

const std = @import("std");

const block = @import("block.zig");
const region = @import("region.zig");
const bin = @import("bin.zig");

block_allocator: block.BlockAllocator = undefined,
region_allocator: region.RegionAllocator = undefined,
bins: [bin.Count]bin.Bin = undefined,

gpa: std.mem.Allocator,

pub fn init(gpa: std.mem.Allocator) !*Heap {
    const heap = try gpa.create(Heap);
    heap.* = Heap{
        .gpa = gpa,
    };

    heap.region_allocator = region.RegionAllocator.init(gpa);
    heap.block_allocator = block.BlockAllocator.init(&heap.region_allocator);

    for (0..bin.Count) |i| {
        heap.bins[i] = bin.Bin.init(&heap.block_allocator, bin.Classes[i]);
    }

    return heap;
}

pub fn allocate(heap: *Heap, comptime T: type) !*T {
    const class_index = bin.getClassIndex(@sizeOf(T));

    var bin_ = &heap.bins[class_index];
    const cell = try bin_.acquireCell(bin.Classes[class_index]);
    // return @ptrFromInt(@intFromPtr(cell));
    return @ptrCast(cell);
}
