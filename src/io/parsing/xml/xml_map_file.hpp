#pragma once

#include <filesystem>   // path
#include <pugixml.hpp>  // xml_document

#include "aliases/shared.hpp"
#include "io/parsing/map_file.hpp"
#include "xml_map_object.hpp"

namespace Tactile::IO {

class XmlMapFile final : public IMapFile
{
 public:
  explicit XmlMapFile(const std::filesystem::path& path);

  [[nodiscard]] auto GetMap() const -> Object override;

  [[nodiscard]] auto GetLayers() const -> Objects override;

  [[nodiscard]] auto GetTilesets() const -> Objects override;

  [[nodiscard]] auto ContainsLayers() const -> bool override;

  [[nodiscard]] auto ContainsTilesets() const -> bool override;

 private:
  Shared<pugi::xml_document> mDocument;

  [[nodiscard]] auto GetMapNode() const -> XmlMapObject;
};

}  // namespace Tactile::IO
