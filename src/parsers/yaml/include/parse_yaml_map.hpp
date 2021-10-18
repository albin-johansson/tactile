#pragma once

#include <filesystem>  // path

#include <expected.hpp>  // expected

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "api.hpp"

namespace Tactile::IO {

[[nodiscard]] TACTILE_YAML_PARSER_API auto ParseYamlMap(const std::filesystem::path& path)
    -> tl::expected<MapData, ParseError>;

}  // namespace Tactile::IO
