#include "open_tmx.hpp"

#include <QtXml>
#include <optional>  // optional

#include "string_utils.hpp"
#include "xml_utils.hpp"

namespace tactile::service {
namespace {

using layer_parse_result =
    std::optional<std::pair<layer_id, std::shared_ptr<core::layer>>>;

[[nodiscard]] auto add_tileset_common(core::map_document* document,
                                      const QDomElement& elem,
                                      const QFileInfo& path,
                                      const tile_id gid) -> bool
{
  const auto tileWidth =
      xml::int_attr<tile_width>(elem, QStringLiteral(u"tilewidth"));
  if (!tileWidth) {
    return false;
  }

  const auto tileHeight =
      xml::int_attr<tile_height>(elem, QStringLiteral(u"tileheight"));
  if (!tileHeight) {
    return false;
  }

  const auto imageElem = elem.firstChildElement(QStringLiteral(u"image"));
  const auto source = imageElem.attribute(QStringLiteral(u"source"));
  const auto absolutePath = path.dir().absoluteFilePath(source);

  auto tileset = std::make_shared<core::tileset>(gid,
                                                 absolutePath,
                                                 *tileWidth,
                                                 *tileHeight);
  tileset->set_name(elem.attribute(QStringLiteral(u"name")));
  tileset->set_path(absolutePath);

  document->add_tileset(std::move(tileset));

  return true;
}

[[nodiscard]] auto add_tileset(core::map_document* document,
                               const QFileInfo& path,
                               const QDomNode& tilesetNode) -> bool
{
  const auto elem = xml::to_elem(tilesetNode);
  Q_ASSERT(elem.tagName() == QStringView{u"tileset"});

  const auto gid = xml::int_attr<tile_id>(elem, QStringLiteral(u"firstgid"));
  if (!gid) {
    return false;
  }

  bool ok;
  if (elem.hasAttribute(QStringLiteral(u"source"))) {
    const auto source = elem.attribute(QStringLiteral(u"source"));
    const auto external = xml::from_file(path.dir().absoluteFilePath(source));
    if (!external) {
      ok = false;
    } else {
      ok =
          add_tileset_common(document, external->documentElement(), path, *gid);
    }
  } else {
    ok = add_tileset_common(document, elem, path, *gid);
  }

  return ok;
}

[[nodiscard]] auto make_layer(const QDomNode& layerNode) -> layer_parse_result
{
  const auto elem = xml::to_elem(layerNode);
  Q_ASSERT(elem.tagName() == QStringLiteral(u"layer"));

//  const auto type = elem.attribute(QStringLiteral(u"type"));
//  if (type != QStringLiteral(u"tilelayer")) {
//    return std::nullopt;
//  }

  const auto id = xml::int_attr<layer_id>(elem, QStringLiteral(u"id"));
  if (!id) {
    return std::nullopt;
  }

  const auto rows = xml::int_attr<row_t>(elem, QStringLiteral(u"height"));
  if (!rows) {
    return std::nullopt;
  }

  const auto cols = xml::int_attr<col_t>(elem, QStringLiteral(u"width"));
  if (!cols) {
    return std::nullopt;
  }

  auto layer = std::make_shared<core::layer>(*rows, *cols);
  layer->set_visible(xml::int_attr(elem, QStringLiteral(u"visible"), 1) == 1);
  layer->set_opacity(xml::double_attr(elem, QStringLiteral(u"opacity"), 1.0));
  layer->set_name(
      elem.attribute(QStringLiteral(u"name"), QStringLiteral(u"Layer")));

  const auto data = elem.firstChildElement(QStringLiteral(u"data"));
  const auto tiles = data.text().split(u',');

  for (int index{0}; const auto& value : tiles) {
    const core::position pos{row_t{index / cols->get()},
                             col_t{index % cols->get()}};
    layer->set_tile(pos, to_int<tile_id>(value));
    ++index;
  }

  layer_parse_result result;
  result.emplace(*id, layer);
  return result;
}

}  // namespace

auto open_tmx_map(const QFileInfo& path) -> core::map_document*
{
  const auto tmx = xml::from_file(path);
  if (!tmx) {
    return nullptr;
  }

  const auto root = tmx->documentElement();
  bool ok{true};

  auto* document = new core::map_document{};

  const auto nextLayerId =
      xml::int_attr<layer_id>(root, QStringLiteral(u"nextlayerid"));
  if (!nextLayerId) {
    return nullptr;
  }

  document->set_next_layer_id(*nextLayerId);

  xml::each_elem(root, QStringLiteral(u"tileset"), [&](const QDomNode& node) {
    add_tileset(document, path, node);
  });

  bool first{true};
  xml::each_elem(root, QStringLiteral(u"layer"), [&](const QDomNode& node) {
    if (auto result = make_layer(node); result) {
      auto [id, layer] = *result;
      document->add_layer(id, layer);
      if (first) {
        first = false;
        document->select_layer(id);
      }
    } else {
      ok = false;
    }
  });

  if (ok) {
    return document;
  } else {
    delete document;
    return nullptr;
  }
}

}  // namespace tactile::service
