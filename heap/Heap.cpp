#include "heap/Heap.h"
#include "heap/Bin.h"


namespace Nyx {
    void *Heap::allocate(size_t size) {
        const size_t bin_index = Bin::getBinIndex(size);
        Bin &bin = m_bins[bin_index];

        bin.allocate(Bin::Sizes[bin_index]);
    }
} // namespace Nyx
