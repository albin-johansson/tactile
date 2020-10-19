#include "open_tmx.hpp"

#include <QtXml>

#include "string_utils.hpp"
#include "tactile_error.hpp"
#include "xml_utils.hpp"

namespace tactile::service {
namespace {

void add_tileset_common(core::map_document* document,
                        const QDomElement& elem,
                        const QFileInfo& path,
                        tile_id gid)
{
  const auto tileWidth =
      xml::int_attr<tile_width>(elem, QStringLiteral(u"tilewidth"));
  const auto tileHeight =
      xml::int_attr<tile_height>(elem, QStringLiteral(u"tileheight"));

  const auto imageElem = elem.firstChildElement(QStringLiteral(u"image"));
  const auto source = imageElem.attribute(QStringLiteral(u"source"));
  const auto absolutePath = path.dir().absoluteFilePath(source);

  auto tileset =
      std::make_shared<core::tileset>(gid, absolutePath, tileWidth, tileHeight);
  tileset->set_name(elem.attribute(QStringLiteral(u"name")));
  tileset->set_path(absolutePath);

  document->add_tileset(std::move(tileset));
}

void add_tileset(core::map_document* document,
                 const QFileInfo& path,
                 const QDomNode& tilesetNode)
{
  const auto elem = xml::to_elem(tilesetNode);
  Q_ASSERT(elem.tagName() == QStringLiteral(u"tileset"));

  const auto gid = xml::int_attr<tile_id>(elem, QStringLiteral(u"firstgid"));

  if (elem.hasAttribute(QStringLiteral(u"source"))) {
    const auto source = elem.attribute(QStringLiteral(u"source"));
    const auto external = xml::from_file(path.dir().absoluteFilePath(source));
    add_tileset_common(document, external.documentElement(), path, gid);
  } else {
    add_tileset_common(document, elem, path, gid);
  }
}

[[nodiscard]] auto make_layer(const QDomNode& layerNode)
    -> std::pair<layer_id, core::layer>
{
  const auto elem = xml::to_elem(layerNode);
  Q_ASSERT(elem.tagName() == QStringLiteral(u"layer"));

  const auto id = xml::int_attr<layer_id>(elem, QStringLiteral(u"id"));
  const auto rows = xml::int_attr<core::row_t>(elem, QStringLiteral(u"height"));
  const auto cols = xml::int_attr<core::col_t>(elem, QStringLiteral(u"width"));

  core::layer layer{rows, cols};
  layer.set_visible(xml::int_attr(elem, QStringLiteral(u"visible"), 1) == 1);
  layer.set_opacity(xml::double_attr(elem, QStringLiteral(u"opacity"), 1.0));

  const auto data = elem.firstChildElement(QStringLiteral(u"data"));
  const auto tiles = data.text().split(u',');

  for (int index{0}; const auto& value : tiles) {
    const core::position pos{core::row_t{index / cols.get()},
                             core::col_t{index % cols.get()}};
    layer.set_tile(pos, to_int<tile_id>(value));
    ++index;
  }

  return {id, layer};
}

}  // namespace

auto open_tmx_map(const QFileInfo& path) -> core::map_document*
{
  const auto tmx = xml::from_file(path);
  const auto root = tmx.documentElement();

  auto* document = new core::map_document{};
  document->set_next_layer_id(
      xml::int_attr<layer_id>(root, QStringLiteral(u"nextlayerid")));

  xml::each_elem(root, QStringLiteral(u"tileset"), [&](const QDomNode& node) {
    add_tileset(document, path, node);
  });

  bool first{true};
  xml::each_elem(root, QStringLiteral(u"layer"), [&](const QDomNode& node) {
    auto [id, layer] = make_layer(node);
    document->add_layer(id, std::move(layer));
    if (first) {
      first = false;
      document->select_layer(id);
    }
  });

  return document;
}

}  // namespace tactile::service
