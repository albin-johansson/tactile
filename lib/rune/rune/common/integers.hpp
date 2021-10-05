#ifndef RUNE_INTEGERS_HPP_
#define RUNE_INTEGERS_HPP_

#include <centurion.hpp>  // int{}_t, uint{}_t
#include <cstddef>        // size_t

namespace rune {

/// \addtogroup core
/// \{

using usize = std::size_t;
using ulonglong = unsigned long long;

using int8 = cen::i8;
using int16 = cen::i16;
using int32 = cen::i32;
using int64 = cen::i64;

using uint8 = cen::u8;
using uint16 = cen::u16;
using uint32 = cen::u32;
using uint64 = cen::u64;

/// \} End of group core

}  // namespace rune

#endif  // RUNE_INTEGERS_HPP_
