// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>   // exception
#include <expected>    // expected
#include <filesystem>  // path
#include <fstream>     // ifstream, ofstream
#include <iomanip>     // setw
#include <ios>         // ios
#include <optional>    // optional, nullopt

#include <nlohmann/json.hpp>

#include "tactile/base/debug/error_code.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

/**
 * Attempts to parse a JSON file.
 *
 * \param path The file path to the JSON file.
 *
 * \return
 * A JSON object if successful; an empty optional otherwise.
 */
[[nodiscard]]
inline auto load_json(const std::filesystem::path& path) -> std::optional<nlohmann::json>
{
  runtime::log(LogLevel::kDebug, "Parsing JSON: {}", path.string());

  try {
    std::ifstream stream {path, std::ios::in};

    if (stream.good()) {
      nlohmann::json json {};
      stream >> json;
      return json;
    }

    runtime::log(LogLevel::kError, "Could not open JSON file");
  }
  catch (const std::exception& error) {
    runtime::log(LogLevel::kError, "JSON parse error: {}", error.what());
  }
  catch (...) {
    runtime::log(LogLevel::kError, "JSON parse error");
  }

  return std::nullopt;
}

/**
 * Attempts to save a JSON object to a file.
 *
 * \param path        The destination file path.
 * \param json        The JSON object to save.
 * \param indentation The indentation level to use.
 *
 * \return
 * Nothing if successful; an error code otherwise.
 */
[[nodiscard]]
inline auto save_json(const std::filesystem::path& path,
                      const nlohmann::json& json,
                      const int indentation) -> std::expected<void, ErrorCode>
{
  runtime::log(LogLevel::kDebug, "Saving JSON: {}", path.string());

  try {
    std::ofstream stream {path, std::ios::out | std::ios::trunc};

    if (stream.good()) {
      stream << std::setw(indentation) << json;
      return {};
    }

    runtime::log(LogLevel::kError, "Could not open JSON stream");
  }
  catch (const std::exception& error) {
    runtime::log(LogLevel::kError, "JSON save error: {}", error.what());
  }
  catch (...) {
    runtime::log(LogLevel::kError, "JSON save error");
  }

  return std::unexpected {ErrorCode::kBadFileStream};
}

}  // namespace tactile
