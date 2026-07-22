#pragma once


#include <array>
#include <cstddef>
#include "heap/Bin.h"


namespace Nyx {
    class Heap {
        void *allocate(size_t size);

    private:
        std::array<Bin, Bin::Count> m_bins;
    };
} // namespace Nyx
