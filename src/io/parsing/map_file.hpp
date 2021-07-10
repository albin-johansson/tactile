#pragma once

#include "aliases/maybe.hpp"
#include "io/parsing/map_object.hpp"

namespace Tactile::IO {

class IMapFile
{
 public:
  using Object = IMapObject::Object;
  using Objects = IMapObject::Objects;

  virtual ~IMapFile() noexcept = default;

  [[nodiscard]] virtual auto GetMap() const -> Object = 0;

  [[nodiscard]] virtual auto GetLayers() const -> Objects = 0;

  [[nodiscard]] virtual auto GetTilesets() const -> Objects = 0;

  [[nodiscard]] virtual auto ContainsLayers() const -> bool = 0;

  [[nodiscard]] virtual auto ContainsTilesets() const -> bool = 0;
};

}  // namespace Tactile::IO
