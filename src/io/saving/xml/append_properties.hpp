#pragma once

#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

#include "core/properties/property_context.hpp"

namespace Tactile::IO {

void AppendProperties(const IPropertyContext& context,
                      pugi::xml_node& node,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
