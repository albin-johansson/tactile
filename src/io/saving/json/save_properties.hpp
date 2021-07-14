#pragma once

#include <filesystem>  // path
#include <json.hpp>    // json

#include "core/properties/property_context.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveProperties(const IPropertyContext& context,
                                  const std::filesystem::path& dir)
    -> nlohmann::json;

}  // namespace Tactile::IO
