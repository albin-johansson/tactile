#include "xml_engine.hpp"

#include <cstddef>  // size_t

#include "tactile_qstring.hpp"

namespace tactile::tmx {

auto xml_engine::tilesets(const object_type& root) -> std::vector<object_type>
{
  return collect(root, TACTILE_QSTRING(u"tileset"));
}

auto xml_engine::layers(const object_type& root) -> std::vector<object_type>
{
  return collect(root, TACTILE_QSTRING(u"layer"));
}

auto xml_engine::root(const document_type& document) -> object_type
{
  return object_type{document.documentElement()};
}

auto xml_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return xml::from_file(path);
}

auto xml_engine::add_tiles(core::tile_layer& layer,
                           const object_type& element,
                           parse_error& error) -> bool
{
  const auto data = element->firstChildElement(TACTILE_QSTRING(u"data"));
  const auto tiles = data.text().split(u',');
  const auto nCols = layer.col_count().get();

  int index{0};
  for (const auto& value : tiles) {
    bool ok{};
    const tile_id id{value.toInt(&ok)};

    if (!ok) {
      error = parse_error::layer_could_not_parse_tile;
      return false;
    }

    const core::position pos{row_t{index / nCols}, col_t{index % nCols}};
    layer.set_tile(pos, id);
    ++index;
  }

  return true;
}

auto xml_engine::tileset_image_relative_path(const object_type& object)
    -> maybe<QString>
{
  const auto imageElem = object->firstChildElement(TACTILE_QSTRING(u"image"));
  const auto path = imageElem.attribute(TACTILE_QSTRING(u"source"));
  if (!path.isNull()) {
    return path;
  } else {
    return std::nullopt;
  }
}

auto xml_engine::collect(const object_type& root, const QString& key)
    -> std::vector<object_type>
{
  const auto elements = root->elementsByTagName(key);
  const auto count = elements.count();

  std::vector<object_type> vector;
  vector.reserve(static_cast<std::size_t>(count));

  for (auto i = 0; i < count; ++i) {
    const auto& node = elements.at(i);
    Q_ASSERT(node.isElement());

    vector.emplace_back(node.toElement());
  }

  return vector;
}

}  // namespace tactile::tmx
