#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"
#include "core/properties/property_context.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveProperties(const IPropertyContext& context,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
