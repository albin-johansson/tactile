#pragma once

#include <QtXml>
#include <concepts>  // invocable

#include "element_id.hpp"
#include "layer.hpp"
#include "map_file_type.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "xml_element.hpp"
#include "xml_utils.hpp"

namespace tactile::tmx {

class xml_engine final
{
 public:
  using document_type = QDomDocument;
  using object_type = xml_element;
  inline constexpr static map_file_type fileType = map_file_type::tmx;

  [[nodiscard]] static auto root(const document_type& document) -> object_type;

  [[nodiscard]] static auto from_file(const QFileInfo& path)
      -> maybe<document_type>;

  [[nodiscard]] static auto add_tiles(core::layer& layer,
                                      const object_type& element,
                                      parse_error& error) -> bool;

  template <std::invocable<const object_type&> T>
  void each_tileset(const object_type& root, T&& callable)
  {
    const auto elements = root->elementsByTagName(QStringLiteral(u"tileset"));
    const auto count = elements.count();
    for (auto i = 0; i < count; ++i) {
      const auto& node = elements.at(i);
      Q_ASSERT(node.isElement());

      const object_type elem{node.toElement()};
      Q_ASSERT(elem->tagName() == QStringView{u"tileset"});

      if (!callable(elem)) {
        return;
      }
    }
  }

  template <std::invocable<const object_type&> T>
  void each_layer(const object_type& root, T&& callable)
  {
    const auto elements = root->elementsByTagName(QStringLiteral(u"layer"));
    const auto count = elements.count();
    for (auto i = 0; i < count; ++i) {
      const auto& node = elements.at(i);
      Q_ASSERT(node.isElement());

      const object_type elem{node.toElement()};
      Q_ASSERT(elem->tagName() == QStringLiteral(u"layer"));

      callable(elem);
    }
  }
};

}  // namespace tactile::tmx
