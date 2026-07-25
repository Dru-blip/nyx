#include "heap/Heap.h"
#include "heap/Bin.h"
#include "heap/RegionAllocator.h"


namespace Nyx {
    Heap::Heap() : m_region_allocator(RegionAllocator()), m_block_allocator(m_region_allocator) {
        for (auto &bin: m_bins) {
            bin.set_block_allocator(m_block_allocator);
        }
    }

    void *Heap::allocate_cell(size_t size) {
        const size_t bin_index = Bin::getBinIndex(size);
        Bin &bin = m_bins[bin_index];
        void *ptr = bin.allocate(Bin::Sizes[bin_index]);
        return ptr;
    }
} // namespace Nyx
