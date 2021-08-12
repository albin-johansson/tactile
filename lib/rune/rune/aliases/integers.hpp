#ifndef RUNE_ALIASES_INTEGERS_HPP
#define RUNE_ALIASES_INTEGERS_HPP

#include <centurion.hpp>  // ...
#include <cstddef>        // size_t

namespace rune {

/// \addtogroup core
/// \{

using usize = std::size_t;

using longlong = long long;

using ushort = unsigned short;

/// Unsigned integer.
using uint = unsigned;

/// Unsigned long integer.
using ulong = unsigned long;

/// Used as the argument type to integral literal operators.
using ulonglong = unsigned long long;

/// 8-bit signed integer.
using int8 = cen::i8;

/// 16-bit signed integer.
using int16 = cen::i16;

/// 32-bit signed integer.
using int32 = cen::i32;

/// 64-bit signed integer.
using int64 = cen::i64;

/// 8-bit unsigned integer.
using uint8 = cen::u8;

/// 16-bit unsigned integer.
using uint16 = cen::u16;

/// 32-bit unsigned integer.
using uint32 = cen::u32;

/// 64-bit unsigned integer.
using uint64 = cen::u64;

/// \} End of group core

}  // namespace rune

#endif  // RUNE_ALIASES_INTEGERS_HPP
