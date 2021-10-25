#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"

namespace Tactile::IO {

void AppendLayer(pugi::xml_node mapNode,
                 const Layer& layer,
                 const std::filesystem::path& dir,
                 bool humanReadableOutput);

}  // namespace Tactile::IO
