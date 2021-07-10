#pragma once

#include "io/parsing/json/json_map_object.hpp"
#include "io/parsing/map_file.hpp"

namespace Tactile::IO {

class JsonMapFile final : public IMapFile
{
 public:
  explicit JsonMapFile(const std::filesystem::path& path);

  [[nodiscard]] auto GetMap() const -> Object override;

  [[nodiscard]] auto GetLayers() const -> Objects override;

  [[nodiscard]] auto GetTilesets() const -> Objects override;

  [[nodiscard]] auto ContainsLayers() const -> bool override;

  [[nodiscard]] auto ContainsTilesets() const -> bool override;

 private:
  JsonMapObject mRoot;
};

}  // namespace Tactile::IO
