// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception
#include <fstream>    // ifstream
#include <ios>        // ios

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/filesystem.hpp"

namespace tactile {

/**
 * \brief Attempts to stream a file from disk into a container object.
 *
 * \tparam T a type that provides an overloaded operator>> that accepts an istream.
 *
 * \param file the path to the source file.
 *
 * \return the parsed content, or nothing if an error occurred.
 */
template <typename T>
[[nodiscard]] auto stream_file_into(const FilePath& file) -> Maybe<T>
{
  std::ifstream stream {file, std::ios::in};  // NOLINT: shall not be const.
  if (!stream.good()) {
    TACTILE_LOG_ERROR("Could not open file {}", file.string());
    return kNone;
  }

  try {
    T object;
    stream >> object;
    return object;
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Could not read file {}: {}", file.string(), ex.what());
  }

  return kNone;
}

}  // namespace tactile