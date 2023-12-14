// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/common.hpp"

#include <filesystem>  // exists

#include "tactile/foundation/debug/generic_error.hpp"

namespace tactile::yml_format {

auto parse_yaml_file(const FilePath& path) -> Result<YAML::Node>
{
  try {
    TACTILE_LOG_TRACE("Parsing YAML file '{}'", path.string());

    if (!std::filesystem::exists(path)) {
      return unexpected(make_generic_error(GenericError::kNoSuchFile));
    }

    auto root_node = YAML::LoadFile(path.string());

    if (!root_node) {
      return unexpected(make_generic_error(GenericError::kInvalidFile));
    }

    return root_node;
  }
  catch (const YAML::ParserException& ex) {
    TACTILE_LOG_ERROR("Could not parse YAML file: '{}' ({}:{})",
                      ex.what(),
                      ex.mark.line,
                      ex.mark.column);
    return unexpected(make_generic_error(GenericError::kInvalidFile));
  }
  catch (...) {
    TACTILE_LOG_ERROR("An unknown error occurred when parsing YAML file");
    return unexpected(make_generic_error(GenericError::kUnknown));
  }
}

}  // namespace tactile::yml_format
