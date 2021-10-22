#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "parse_ir.hpp"

namespace Tactile::IO {

void AppendObject(pugi::xml_node source,
                  const ObjectData& object,
                  const std::filesystem::path& dir);

}  // namespace Tactile::IO
