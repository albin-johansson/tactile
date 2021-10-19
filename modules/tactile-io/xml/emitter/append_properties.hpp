#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <pugixml.hpp>  // xml_node

#include "../../parse_ir.hpp"

namespace Tactile::IO {

void AppendProperties(pugi::xml_node node,
                      const std::vector<PropertyData>& properties,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
