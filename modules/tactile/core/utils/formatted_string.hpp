#pragma once

#include <array>        // array
#include <string_view>  // string_view

#include <tactile_def.hpp>

#include <fmt/format.h>  // format_to_n

namespace Tactile {

template <usize Capacity = 128>
class FormattedString final
{
 public:
  template <typename... Args>
  explicit FormattedString(const std::string_view fmt, const Args&... args)
  {
    const auto result =
        fmt::format_to_n(mBuffer.begin(), Capacity, fmt::runtime(fmt), args...);
    *result.out = '\0';
    mSize = result.size;
  }

  [[nodiscard]] auto GetData() const noexcept -> CStr
  {
    return mBuffer.data();
  }

  [[nodiscard]] auto GetView() const noexcept -> std::string_view
  {
    return std::string_view{mBuffer.data(), mSize};
  }

  [[nodiscard]] auto GetSize() const noexcept -> usize
  {
    return mBuffer.size();
  }

  [[nodiscard]] constexpr auto GetCapacity() const noexcept -> usize
  {
    return Capacity;
  }

 private:
  std::array<char, Capacity + 1> mBuffer;  // NOLINT
  usize mSize{};
};

}  // namespace Tactile