// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/array.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class StringBuffer final {
 public:
  TACTILE_DEFAULT_COPY(StringBuffer);
  TACTILE_DEFAULT_MOVE(StringBuffer);

  StringBuffer() = default;

  auto operator=(StringView str) -> StringBuffer&;

  void clear();

  [[nodiscard]] auto data() -> char* { return mBuffer.data(); }

  [[nodiscard]] auto size() const noexcept -> usize { return mBuffer.size(); }

  [[nodiscard]] auto as_string() const -> String;
  [[nodiscard]] auto as_string_view() const -> StringView;

 private:
  Array<char, 256> mBuffer;
};

[[nodiscard]] auto operator==(const StringBuffer& buffer, StringView str) -> bool;
[[nodiscard]] auto operator==(StringView str, const StringBuffer& buffer) -> bool;

}  // namespace tactile
