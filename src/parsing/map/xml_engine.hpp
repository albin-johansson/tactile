#pragma once

#include <QDomDocument>  // QDomDocument
#include <QString>       // QString
#include <vector>        // vector

#include "element_id.hpp"
#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tile_layer.hpp"
#include "xml_element.hpp"
#include "xml_utils.hpp"

namespace tactile {

class XmlEngine final
{
 public:
  using document_type = QDomDocument;
  using object_type = XmlElement;

  // clang-format off

  [[nodiscard]]
  static auto Root(const document_type& document) -> object_type;

  [[nodiscard]]
  static auto FromFile(const QFileInfo& path) -> Maybe<document_type>;

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
      -> Maybe<QString>;

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
  static auto Collect(const object_type& root, const QString& key)
      -> std::vector<object_type>;

  static auto HasChild(const object_type& obj, const QString& tag) -> bool;

  [[nodiscard]] static auto AssumeStringProperty(const object_type& object)
      -> bool;
};

}  // namespace tactile
