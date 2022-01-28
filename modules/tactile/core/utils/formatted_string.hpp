#pragma once

#include <array>        // array
#include <string_view>  // string_view

#include <fmt/format.h>

#include "tactile_def.hpp"

namespace tactile {

template <usize Capacity = 128>
class formatted_string final {
 public:
  template <typename... Args>
  explicit formatted_string(const std::string_view fmt, const Args&... args)
  {
    const auto result =
        fmt::format_to_n(mBuffer.begin(), Capacity, fmt::runtime(fmt), args...);
    *result.out = '\0';
    mSize = result.size;
  }

  [[nodiscard]] auto data() const noexcept -> CStr { return mBuffer.data(); }

  [[nodiscard]] auto view() const noexcept -> std::string_view
  {
    return std::string_view{mBuffer.data(), mSize};
  }

  [[nodiscard]] auto size() const noexcept -> usize { return mBuffer.size(); }

  [[nodiscard]] constexpr auto capacity() const noexcept -> usize { return Capacity; }

 private:
  std::array<char, Capacity + 1> mBuffer;  // NOLINT
  usize mSize{};
};

}  // namespace tactile