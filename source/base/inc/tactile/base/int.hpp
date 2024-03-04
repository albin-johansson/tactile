// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t, ptrdiff_t
#include <cstdint>  // int{}_t, uint{}_t

namespace tactile {

using usize = std::size_t;
using ssize = std::ptrdiff_t;

using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long int;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

}  // namespace tactile
