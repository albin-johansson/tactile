#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "tactile_io.hpp"

namespace Tactile::IO {

void AppendProperties(pugi::xml_node node,
                      const Map& map,
                      const std::filesystem::path& dir);

void AppendProperties(pugi::xml_node node,
                      const Layer& layer,
                      const std::filesystem::path& dir);

void AppendProperties(pugi::xml_node node,
                      const Tileset& tileset,
                      const std::filesystem::path& dir);

void AppendProperties(pugi::xml_node node,
                      const Tile& tile,
                      const std::filesystem::path& dir);

void AppendProperties(pugi::xml_node node,
                      const Object& object,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
