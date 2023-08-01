/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace tactile {

/// A simple type used to indicate the result of functions that normally return `void`.
class Result final {
 public:
  [[nodiscard]] constexpr static auto make_success() noexcept -> Result
  {
    return Result {true};
  }

  [[nodiscard]] constexpr static auto make_failure() noexcept -> Result
  {
    return Result {false};
  }

  [[nodiscard]] constexpr auto succeeded() const noexcept -> bool { return mSuccess; }
  [[nodiscard]] constexpr auto failed() const noexcept -> bool { return !succeeded(); }

  [[nodiscard]] constexpr explicit operator bool() const noexcept { return mSuccess; }

  [[nodiscard]] auto operator==(const Result& other) const noexcept -> bool = default;

 private:
  bool mSuccess {false};

  constexpr explicit Result(bool success) noexcept
      : mSuccess {success}
  {
  }
};

inline constexpr Result success = Result::make_success();  // NOLINT
inline constexpr Result failure = Result::make_failure();  // NOLINT

}  // namespace tactile