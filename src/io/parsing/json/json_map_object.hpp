#pragma once

#include <filesystem>   // path
#include <json.hpp>     // json
#include <string_view>  // string_view

#include "io/parsing/map_object.hpp"

namespace Tactile::IO {

class JsonMapObject : public IMapObject
{
 public:
  explicit JsonMapObject(const std::filesystem::path& path);

  explicit JsonMapObject(nlohmann::json json);

  [[nodiscard]] auto FromFile(const std::filesystem::path& path) const
      -> Object override;

  [[nodiscard]] auto Contains(MapAttribute attr) const -> bool override;

  [[nodiscard]] auto GetInt(MapAttribute attr) const -> Maybe<int> override;

  [[nodiscard]] auto GetFloat(MapAttribute attr) const -> Maybe<float> override;

  [[nodiscard]] auto GetBool(MapAttribute attr) const -> Maybe<bool> override;

  [[nodiscard]] auto GetString(MapAttribute attr) const
      -> Maybe<std::string> override;

  [[nodiscard]] auto GetArray(MapAttribute attr) const -> Objects override;

  [[nodiscard]] auto GetTilesetImageRelativePath() const
      -> Maybe<std::filesystem::path> override;

  [[nodiscard]] auto IsTileLayer() const -> bool override;

  [[nodiscard]] auto IsObjectLayer() const -> bool override;

  [[nodiscard]] auto IsImplicitStringProperty() const -> bool override;

  [[nodiscard]] auto IsPoint() const -> bool override;

  [[nodiscard]] auto HasType() const -> bool override;

  [[nodiscard]] auto GetTileData(col_t nCols, TileMatrix& matrix) const
      -> ParseError override;

  [[nodiscard]] auto GetObjects() const -> Objects override;

  [[nodiscard]] auto GetProperties() const -> Objects override;

  [[nodiscard]] auto GetExternalTileset() const -> Object override;

  [[nodiscard]] auto GetArray(std::string_view key) const -> Objects;

  [[nodiscard]] auto GetData() const -> const nlohmann::json&
  {
    return mJson;
  }

  [[nodiscard]] static auto ToKey(MapAttribute attr) -> std::string_view;

 private:
  nlohmann::json mJson;
};

}  // namespace Tactile::IO
