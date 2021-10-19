#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "../../parse_ir.hpp"

namespace Tactile::IO {

void AppendLayer(pugi::xml_node mapNode,
                 const LayerData& layer,
                 const std::filesystem::path& dir);

}  // namespace Tactile::IO
