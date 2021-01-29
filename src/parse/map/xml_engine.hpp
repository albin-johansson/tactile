#pragma once

#include <QString>  // QString
#include <QtXml>    // QDomDocument
#include <vector>   // vector

#include "element_id.hpp"
#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tile_layer.hpp"
#include "xml_element.hpp"
#include "xml_utils.hpp"

namespace tactile::parse {

class xml_engine final
{
 public:
  using document_type = QDomDocument;
  using object_type = xml_element;

  // clang-format off

  [[nodiscard]]
  static auto root(const document_type& document) -> object_type;

  [[nodiscard]]
  static auto from_file(const QFileInfo& path) -> maybe<document_type>;

  [[nodiscard]]
  static auto tilesets(const object_type& root) -> std::vector<object_type>;

  [[nodiscard]]
  static auto layers(const object_type& root) -> std::vector<object_type>;

  [[nodiscard]]
  static auto properties(const object_type& object) -> std::vector<object_type>;

  [[nodiscard]]
  static auto objects(const object_type& object) -> std::vector<object_type>;

  [[nodiscard]]
  static auto tiles(const object_type& object, row_t nRows, col_t nCols, parse_error& error)
      -> core::tile_matrix;

  [[nodiscard]]
  static auto property_type(const object_type& object) -> QString;

  [[nodiscard]]
  static auto contains_tilesets(const object_type& object) -> bool;

  [[nodiscard]]
  static auto tileset_image_relative_path(const object_type& object)
      -> maybe<QString>;

  [[nodiscard]]
  static auto validate_layer_type(const object_type& object) -> bool;

  [[nodiscard]]
  static auto contains_layers(const object_type& object) -> bool;

  [[nodiscard]]
  static auto is_tile_layer(const object_type& object) -> bool;

  [[nodiscard]]
  static auto is_object_layer(const object_type& object) -> bool;

  [[nodiscard]]
  static auto is_point(const object_type& object) -> bool;

  // clang-format on

 private:
  static auto collect(const object_type& root, const QString& key)
      -> std::vector<object_type>;

  static auto has_child(const object_type& obj, const QString& tag) -> bool;

  [[nodiscard]] static auto assume_string_property(const object_type& object)
      -> bool;
};

}  // namespace tactile::parse
