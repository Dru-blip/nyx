
#include "runtime/ops/Integer.h"

namespace Nyx::Handlers {
    using namespace Nyx::Ops;

    static inline Value handle_add(const Value a, const Value b) { return Ops::Integer::add(a, b); }

    static inline Value handle_sub(const Value a, const Value b) { return Ops::Integer::sub(a, b); }

    static inline Value handle_mul(const Value a, const Value b) { return Ops::Integer::mul(a, b); }

    static inline Value handle_div(const Value a, const Value b) { return Ops::Integer::div(a, b); }
} // namespace Nyx::Handlers
