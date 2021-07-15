#pragma once

#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

#include "core/map/layers/layer.hpp"

namespace Tactile::IO {

void AppendLayer(const ILayer& layer,
                 pugi::xml_node& mapNode,
                 const std::filesystem::path& dir);

}  // namespace Tactile::IO
