#include "tiled_tmx_parser.hpp"

#include <utility>  // exchange

#include "layer_id.hpp"
#include "tileset.hpp"
#include "xml_utils.hpp"

namespace tactile {

tiled_tmx_parser::tiled_tmx_parser(const QFileInfo& path)
{
  if (const auto xml = open_file(path); xml) {
    m_document = new core::map_document{};
    m_document->set_path(path);

    const auto root = xml->documentElement();
    if (!parse_next_layer_id(root)) {
      return;
    }

    if (!parse_tilesets(root, path)) {
      return;
    }

    if (!parse_layers(root)) {
      return;
    }
  }
}

tiled_tmx_parser::~tiled_tmx_parser() noexcept
{
  delete m_document;
}

auto tiled_tmx_parser::take_document() -> core::map_document*
{
  return std::exchange(m_document, nullptr);
}

auto tiled_tmx_parser::error_code() const -> parse_error
{
  return m_error;
}

tiled_tmx_parser::operator bool() const noexcept
{
  return m_error == parse_error::none;
}

auto tiled_tmx_parser::open_file(const QFileInfo& path) -> maybe<QDomDocument>
{
  if (!path.exists()) {
    m_error = parse_error::map_file_not_found;
    return std::nullopt;
  }

  auto tmx = xml::from_file(path);

  if (!tmx) {
    m_error = parse_error::could_not_parse_file;
  }

  return tmx;
}

auto tiled_tmx_parser::parse_next_layer_id(const QDomElement& root) -> bool
{
  const auto result =
      xml::int_attr<layer_id>(root, QStringLiteral(u"nextlayerid"));
  if (result) {
    m_document->set_next_layer_id(layer_id{*result});
    return true;
  } else {
    return set_error(parse_error::map_missing_next_layer_id);
  }
}

auto tiled_tmx_parser::parse_tilesets(const QDomElement& root,
                                      const QFileInfo& path) -> bool
{
  const auto elements = root.elementsByTagName(QStringLiteral(u"tileset"));
  const auto count = elements.count();
  for (auto i = 0; i < count; ++i) {
    const auto& node = elements.at(i);

    const auto elem = xml::to_elem(node);
    Q_ASSERT(elem.tagName() == QStringView{u"tileset"});

    const auto firstGid = parse_tileset_first_gid(elem);
    if (!firstGid) {
      return false;
    }

    if (elem.hasAttribute(QStringLiteral(u"source"))) {
      if (!parse_external_tileset(elem, path, *firstGid)) {
        return false;
      }
    } else {
      if (!parse_tileset_common(elem, path, *firstGid)) {
        return false;
      }
    }
  }

  return true;
}

auto tiled_tmx_parser::parse_tileset_first_gid(const QDomElement& elem)
    -> maybe<tile_id>
{
  const auto gid = xml::int_attr<tile_id>(elem, QStringLiteral(u"firstgid"));

  if (!gid) {
    m_error = parse_error::tileset_missing_first_gid;
    return std::nullopt;
  }

  return gid;
}

auto tiled_tmx_parser::parse_external_tileset(const QDomElement& elem,
                                              const QFileInfo& path,
                                              const tile_id firstGid) -> bool
{
  const auto source = elem.attribute(QStringLiteral(u"source"));
  const auto external = xml::from_file(path.dir().absoluteFilePath(source));
  if (!external) {
    return set_error(parse_error::could_not_read_external_tileset);
  } else {
    return parse_tileset_common(external->documentElement(), path, firstGid);
  }
}

auto tiled_tmx_parser::parse_tileset_common(const QDomElement& elem,
                                            const QFileInfo& path,
                                            const tile_id firstGid) -> bool
{
  const auto tw = xml::int_attr<tile_width>(elem, QStringLiteral(u"tilewidth"));
  if (!tw) {
    return set_error(parse_error::tileset_missing_tile_width);
  }

  const auto th =
      xml::int_attr<tile_height>(elem, QStringLiteral(u"tileheight"));
  if (!th) {
    return set_error(parse_error::tileset_missing_tile_height);
  }

  const auto imageElem = elem.firstChildElement(QStringLiteral(u"image"));
  const auto relativePath = imageElem.attribute(QStringLiteral(u"source"));
  if (relativePath.isNull()) {
    return set_error(parse_error::tileset_missing_image_path);
  }

  const auto absolutePath = path.dir().absoluteFilePath(relativePath);
  if (!QFileInfo{absolutePath}.exists()) {
    return set_error(parse_error::external_tileset_does_not_exist);
  }

  try {
    auto tileset =
        std::make_shared<core::tileset>(firstGid, absolutePath, *tw, *th);

    const auto name = elem.attribute(QStringLiteral(u"name"));
    if (name.isNull()) {
      return set_error(parse_error::tileset_missing_name);
    }

    tileset->set_name(name);
    tileset->set_path(absolutePath);

    m_document->add_tileset(std::move(tileset));
  } catch (...) {
    return set_error(parse_error::could_not_create_tileset);
  }

  return true;
}

auto tiled_tmx_parser::parse_layers(const QDomElement& root) -> bool
{
  bool first{true};

  const auto elements = root.elementsByTagName(QStringLiteral(u"layer"));
  const auto count = elements.count();
  for (auto i = 0; i < count; ++i) {
    const auto& node = elements.at(i);

    const auto elem = xml::to_elem(node);
    Q_ASSERT(elem.tagName() == QStringLiteral(u"layer"));

    //  const auto type = elem.attribute(QStringLiteral(u"type"));
    //  if (type != QStringLiteral(u"tilelayer")) {
    //    return std::nullopt;
    //  }

    const auto id = xml::int_attr<layer_id>(elem, QStringLiteral(u"id"));
    if (!id) {
      return set_error(parse_error::layer_missing_id);
    }

    const auto rows = xml::int_attr<row_t>(elem, QStringLiteral(u"height"));
    if (!rows) {
      return set_error(parse_error::layer_missing_height);
    }

    const auto cols = xml::int_attr<col_t>(elem, QStringLiteral(u"width"));
    if (!cols) {
      return set_error(parse_error::layer_missing_width);
    }

    auto layer = std::make_shared<core::layer>(*rows, *cols);
    layer->set_visible(xml::int_attr(elem, QStringLiteral(u"visible"), 1) == 1);
    layer->set_opacity(xml::double_attr(elem, QStringLiteral(u"opacity"), 1.0));
    layer->set_name(
        elem.attribute(QStringLiteral(u"name"), QStringLiteral(u"Layer")));

    const auto data = elem.firstChildElement(QStringLiteral(u"data"));
    const auto tiles = data.text().split(u',');

    for (int index{0}; const auto& value : tiles) {
      bool ok{};
      const auto tileId = value.toInt(&ok);

      if (!ok) {
        return set_error(parse_error::layer_could_not_parse_tile);
      }

      const core::position pos{row_t{index / cols->get()},
                               col_t{index % cols->get()}};
      layer->set_tile(pos, tile_id{tileId});
      ++index;
    }

    m_document->add_layer(*id, layer);
    if (first) {
      first = false;
      m_document->select_layer(*id);  // TODO make this the default behaviour
    }
  }

  return true;
}

}  // namespace tactile
