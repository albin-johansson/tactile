#pragma once

#include <filesystem>  // path
#include <optional>    // optional

#include "../parse_error.hpp"
#include "../parse_ir.hpp"

#ifdef WIN32
#ifdef TACTILE_YAML_PARSER_EXPORT
#define TACTILE_YAML_PARSER_API __declspec(dllexport)
#else
#define TACTILE_YAML_PARSER_API __declspec(dllimport)
#endif  // TACTILE_YAML_PARSER_EXPORT
#else
#define TACTILE_YAML_PARSER_API
#endif  // WIN32

namespace Tactile::IO {

[[nodiscard]] TACTILE_YAML_PARSER_API auto ParseYamlMap(const std::filesystem::path& path,
                                                        ParseError* error = nullptr)
    -> std::optional<MapData>;

}  // namespace Tactile::IO
