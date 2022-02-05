#pragma once

#include <algorithm>    // min
#include <array>        // array
#include <string_view>  // string_view

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "tactile.hpp"

namespace tactile {

template <usize Capacity = 128>
class formatted_string final {
 public:
  template <typename... Args>
  explicit formatted_string(const std::string_view fmt, const Args&... args)
  {
    const auto result =
        fmt::format_to_n(mBuffer.begin(), Capacity, fmt::runtime(fmt), args...);
    *result.out = '\0'; /* Ensure null-terminator */
    mSize = ((std::min))(result.size, Capacity);
  }

  [[nodiscard]] auto data() const noexcept -> c_str { return mBuffer.data(); }

  [[nodiscard]] auto view() const noexcept -> std::string_view
  {
    return std::string_view{mBuffer.data(), mSize};
  }

  [[nodiscard]] auto size() const noexcept -> usize { return view().size(); }

  [[nodiscard]] constexpr auto capacity() const noexcept -> usize { return Capacity; }

 private:
  std::array<char, Capacity + 1> mBuffer;  // NOLINT
  usize mSize{};
};

}  // namespace tactile