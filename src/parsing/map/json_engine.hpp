#pragma once

#include <QFileInfo>      // QFileInfo
#include <QJsonArray>     // QJsonArray
#include <QJsonDocument>  // QJsonDocument
#include <QJsonValue>     // QJsonValue
#include <vector>         // vector

#include "json_element.hpp"
#include "map_parse_data.hpp"
#include "parse_error.hpp"
#include "tile_layer.hpp"

namespace tactile::parse {

class json_engine final
{
 public:
  using document_type = QJsonDocument;
  using object_type = JsonElement;

  // clang-format off

  [[nodiscard]]
  static auto Root(const document_type& document) -> object_type;

  [[nodiscard]]
  static auto FromFile(const QFileInfo& path) -> maybe<document_type>;

  [[nodiscard]]
  static auto Tilesets(const object_type& root) -> std::vector<object_type>;

  [[nodiscard]]
  static auto Layers(const object_type& root) -> std::vector<object_type>;

  [[nodiscard]]
  static auto Properties(const object_type& object) -> std::vector<object_type>;

  [[nodiscard]]
  static auto Objects(const object_type& object) -> std::vector<object_type>;

  [[nodiscard]]
  static auto Tiles(const object_type& object, row_t nRows, col_t nCols, ParseError& error)
      -> core::tile_matrix;

  [[nodiscard]]
  static auto PropertyType(const object_type& object) -> QString;

  [[nodiscard]]
  static auto ContainsTilesets(const object_type& object) -> bool;

  [[nodiscard]]
  static auto TilesetImageRelativePath(const object_type& object)
      -> maybe<QString>;

  [[nodiscard]]
  static auto ValidateLayerType(const object_type& object) -> bool;

  [[nodiscard]]
  static auto ContainsLayers(const object_type& object) -> bool;

  [[nodiscard]]
  static auto IsTileLayer(const object_type& object) -> bool;

  [[nodiscard]]
  static auto IsObjectLayer(const object_type& object) -> bool;

  [[nodiscard]]
  static auto IsPoint(const object_type& object) -> bool;

  // clang-format on

 private:
  static auto Collect(const object_type& root, QStringView key)
      -> std::vector<object_type>;
};

}  // namespace tactile::parse
