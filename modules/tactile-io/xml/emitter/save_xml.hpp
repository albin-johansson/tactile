#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_document

namespace Tactile::IO {

void SaveXml(const pugi::xml_document& xml, const std::filesystem::path& path);

}  // namespace Tactile::IO
