#pragma once

#include <QString>  // QString
#include <QtXml>    // QDomDocument
#include <vector>   // vector

#include "element_id.hpp"
#include "map_file_type.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tile_layer.hpp"
#include "xml_element.hpp"
#include "xml_utils.hpp"

namespace tactile::tmx {

class xml_engine final
{
 public:
  using document_type = QDomDocument;
  using object_type = xml_element;

  inline constexpr static auto fileType = map_file_type::tmx;

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
  static auto add_tiles(core::tile_layer& layer,
                        const object_type& element,
                        parse_error& error) -> bool;

  [[nodiscard]]
  static auto contains_tilesets(const object_type& object) -> bool
  {
    return true;
  }

  [[nodiscard]]
  static auto tileset_image_relative_path(const object_type& object)
      -> maybe<QString>;

  [[nodiscard]]
  static auto validate_layer_type(const object_type& object) -> bool
  {
    return true;
  }

  [[nodiscard]]
  static auto contains_layers(const object_type& object) -> bool
  {
    return true;
  }

  // clang-format on

 private:
  static auto collect(const object_type& root, const QString& key)
      -> std::vector<object_type>;
};

}  // namespace tactile::tmx
