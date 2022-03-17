/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <exception>  // exception

#include <boost/exception/all.hpp>

#include "stacktrace.hpp"

namespace tactile {

class TactileError : public std::exception
{
 public:
  TactileError() noexcept = default;

  explicit TactileError(const char* what) : mWhat{what ? what : "N/A"} {}

  [[nodiscard]] auto what() const noexcept -> const char* override { return mWhat; }

 private:
  const char* mWhat{"N/A"};
};

namespace tags {
struct TraceInfoTag;
}  // namespace tags

using TraceInfo = boost::error_info<tags::TraceInfoTag, boost::stacktrace::stacktrace>;

/**
 * \brief Throws an exception with associated call stack information.
 *
 * \param error the exception to throw.
 *
 * \see trace_info
 */
[[noreturn]] void throw_traced(const auto& error)
{
  throw boost::enable_error_info(error) << TraceInfo{boost::stacktrace::stacktrace()};
}

}  // namespace tactile
