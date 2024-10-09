// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/stacktrace.hpp"

#include <sstream>  // stringstream

#include <boost/stacktrace.hpp>

namespace tactile::core {

auto get_stacktrace() -> std::string
{
  const boost::stacktrace::stacktrace trace {};

  std::stringstream stream;
  stream << trace;

  return stream.str();
}

}  // namespace tactile::core
