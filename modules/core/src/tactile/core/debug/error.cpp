// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/error.hpp"

#include <sstream>  // stringstream
#include <utility>  // move

#include <boost/stacktrace.hpp>

namespace tactile {

Error::Error(tactile::String message)
    : mMessage {std::move(message)}
{
  const boost::stacktrace::stacktrace trace;

  std::stringstream stream;
  stream << trace;

  mTrace = stream.str();
}

auto Error::what() const noexcept -> const char*
{
  return mMessage.c_str();
}

auto Error::get_trace() const -> const String&
{
  return mTrace;
}

}  // namespace tactile