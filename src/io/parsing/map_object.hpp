#pragma once

#include <filesystem>  // path
#include <string>      // string
#include <vector>      // vector

#include "aliases/col.hpp"
#include "aliases/maybe.hpp"
#include "aliases/tile_matrix.hpp"
#include "aliases/unique.hpp"
#include "io/parsing/map_attribute.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

class IMapObject
{
 public:
  using Object = Unique<IMapObject>;
  using Objects = std::vector<Object>;

  virtual ~IMapObject() noexcept = default;

  [[nodiscard]] virtual auto FromFile(const std::filesystem::path& path) const
      -> Object = 0;

  [[nodiscard]] virtual auto Contains(MapAttribute attr) const -> bool = 0;

  [[nodiscard]] virtual auto GetInt(MapAttribute attr) const -> Maybe<int> = 0;

  [[nodiscard]] virtual auto GetFloat(MapAttribute attr) const -> Maybe<float> = 0;

  [[nodiscard]] virtual auto GetBool(MapAttribute attr) const -> Maybe<bool> = 0;

  [[nodiscard]] virtual auto GetString(MapAttribute attr) const
      -> Maybe<std::string> = 0;

  [[nodiscard]] virtual auto GetArray(MapAttribute attr) const -> Objects = 0;

  [[nodiscard]] virtual auto GetTileData(col_t nCols, TileMatrix& matrix) const
      -> ParseError = 0;

  [[nodiscard]] virtual auto GetObjects() const -> Objects = 0;

  [[nodiscard]] virtual auto GetLayers() const -> Objects = 0;

  [[nodiscard]] virtual auto GetProperties() const -> Objects = 0;

  [[nodiscard]] virtual auto GetExternalTileset() const -> Object = 0;

  [[nodiscard]] virtual auto GetTilesetImageRelativePath() const
      -> Maybe<std::filesystem::path> = 0;

  [[nodiscard]] virtual auto IsTileLayer() const -> bool = 0;

  [[nodiscard]] virtual auto IsObjectLayer() const -> bool = 0;

  [[nodiscard]] virtual auto IsGroupLayer() const -> bool = 0;

  [[nodiscard]] virtual auto IsImplicitStringProperty() const -> bool = 0;

  [[nodiscard]] virtual auto IsPoint() const -> bool = 0;

  [[nodiscard]] virtual auto IsEllipse() const -> bool = 0;

  [[nodiscard]] virtual auto HasType() const -> bool = 0;
};

}  // namespace Tactile::IO
