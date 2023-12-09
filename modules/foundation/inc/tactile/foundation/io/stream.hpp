// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception
#include <fstream>    // ifstream, ofstream
#include <iomanip>    // setw
#include <ios>        // ios

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

/**
 * \brief Provides input stream options.
 */
struct TACTILE_FOUNDATION_API StreamFromFileOptions final {
  bool binary_mode : 1 {false};  ///< Set to true to read binary files.
};

/**
 * \brief Attempts to stream a file from disk into a container object.
 *
 * \tparam T a type that provides an overloaded operator>> that accepts an istream.
 *
 * \param path    the path to the source file.
 * \param options the read options.
 *
 * \return the parsed content, or nothing if an error occurred.
 */
template <typename T>
[[nodiscard]] auto stream_from_file(const FilePath& path,
                                    const StreamFromFileOptions& options = {}) -> Maybe<T>
{
  std::ios::openmode stream_flags = std::ios::in;
  if (options.binary_mode) {
    stream_flags |= std::ios::binary;
  }

  std::ifstream stream {path, stream_flags};  // NOLINT: shall not be const.
  if (!stream.good()) {
    TACTILE_LOG_ERROR("Could not open file {}", path.string());
    return kNone;
  }

  try {
    T object;
    stream >> object;
    return object;
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Could not read from file {}: {}", path.string(), ex.what());
  }

  return kNone;
}

/**
 * \brief Provides output stream options.
 */
struct TACTILE_FOUNDATION_API StreamToFileOptions final {
  int indentation {0};       ///< The amount of indentation to use (only for text files).
  bool binary_mode {false};  ///< Set to true to write binary files.
};

/**
 * \brief Attempts to stream an object into a file on disk.
 *
 * \tparam T a type that provides an overloaded operator<< that accepts an ostream.
 *
 * \param content the content that will be written to the file.
 * \param path    the path to the destination file.
 * \param options the write options.
 */
template <typename T>
void stream_to_file(const T& content,
                    const FilePath& path,
                    const StreamToFileOptions& options = {})
{
  std::ios::openmode stream_flags = std::ios::out | std::ios::trunc;
  if (options.binary_mode) {
    stream_flags |= std::ios::binary;
  }

  std::ofstream stream {path, stream_flags};
  if (!stream.good()) {
    TACTILE_LOG_ERROR("Could not open file {}", path.string());
    return;
  }

  try {
    if (!options.binary_mode && options.indentation > 0) {
      stream << std::setw(options.indentation);
    }

    stream << content;
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Could not write to file {}: {}", path.string(), ex.what());
  }
}

}  // namespace tactile