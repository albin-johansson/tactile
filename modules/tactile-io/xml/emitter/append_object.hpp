#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"

namespace Tactile::IO {

void AppendObject(pugi::xml_node source,
                  const Object& object,
                  const std::filesystem::path& dir);

}  // namespace Tactile::IO
