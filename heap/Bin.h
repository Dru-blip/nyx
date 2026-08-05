#pragma once

#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>
#include "heap/BlockAllocator.h"
#include "heap/Cell.h"


namespace Nyx {
    class Bin {
    public:
        static constexpr std::array<uint16_t, 9> Sizes = {32,   64,   128,  256, 512,
                                                          1024, 2048, 4096, 8192};
        static constexpr size_t Count = Sizes.size();

        static size_t getBinIndex(size_t size) {
            const bool is_power_of_two = (size & (size - 1)) == 0;
            if (is_power_of_two && size >= Sizes[0] && size <= Sizes[Count - 1]) {
                return std::log2l(size) - std::log2l(Sizes[0]);
            }
            const size_t nearest_two_power = std::bit_ceil(size);
            return std::log2l(nearest_two_power) - std::log2l(Sizes[0]);
        }


        Bin(BlockAllocator *block_allocator) : m_block_allocator(block_allocator) {};
        Bin() = default;


        void set_block_allocator(BlockAllocator &block_allocator) {
            m_block_allocator = &block_allocator;
        }


        void *allocate(size_t size);
        void try_acquire_block(size_t size);


    private:
        BlockAllocator *m_block_allocator{nullptr};

        Block *m_active{nullptr};
        // stores pointers of used blocks in the bin.
        // but can be UB if the region holding a particular block is destroyed/released.
        std::vector<Block *> m_used;
    };
} // namespace Nyx
