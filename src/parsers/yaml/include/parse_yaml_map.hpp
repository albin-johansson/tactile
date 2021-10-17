#pragma once

#include <expected.hpp>  // expected
#include <filesystem>    // path

#include "api.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] TACTILE_YAML_PARSER_API auto ParseYamlMap(const std::filesystem::path& path)
    -> tl::expected<MapData, ParseError>;

}  // namespace Tactile::IO
