#pragma once

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <concepts>  // invocable

#include "json_element.hpp"
#include "layer.hpp"
#include "map_file_type.hpp"
#include "parse_error.hpp"

namespace tactile::tmx {

class json_engine final
{
 public:
  using document_type = QJsonDocument;
  using object_type = json_element;
  inline constexpr static map_file_type fileType = map_file_type::json;

  [[nodiscard]] static auto root(const document_type& document) -> object_type;

  [[nodiscard]] static auto from_file(const QFileInfo& path)
      -> maybe<document_type>;

  [[nodiscard]] static auto add_tiles(core::layer& layer,
                                      const object_type& element,
                                      parse_error& error) -> bool;

  template <std::invocable<const object_type&> T>
  void each_tileset(const object_type& root, T&& callable)
  {
    const auto array = root->value(u"tilesets").toArray();
    for (const auto& elem : array) {
      const object_type obj{elem.toObject()};
      callable(obj);
    }
  }

  template <std::invocable<const object_type&> T>
  void each_layer(const object_type& root, T&& callable)
  {
    const auto array = root->value(u"layers").toArray();
    for (const auto& elem : array) {
      const object_type object{elem.toObject()};
      callable(object);
    }
  }
};

}  // namespace tactile::tmx
