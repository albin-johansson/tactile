#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "tactile_io.hpp"

namespace tactile::IO {

void AppendObject(pugi::xml_node source,
                  const Object& object,
                  const std::filesystem::path& dir);

}  // namespace tactile::IO
