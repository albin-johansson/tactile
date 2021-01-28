#include "xml_engine.hpp"

#include <cstddef>   // size_t
#include <iterator>  // make_move_iterator

#include "index_to_position.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::parse {

auto xml_engine::tilesets(const object_type& root) -> std::vector<object_type>
{
  return collect(root, TACTILE_QSTRING(u"tileset"));
}

auto xml_engine::layers(const object_type& root) -> std::vector<object_type>
{
  auto layers = collect(root, TACTILE_QSTRING(u"layer"));
  auto objectLayers = collect(root, TACTILE_QSTRING(u"objectgroup"));

  // Move all object layers into the vector with tile layers
  layers.insert(layers.end(),
                std::make_move_iterator(objectLayers.begin()),
                std::make_move_iterator(objectLayers.end()));

  return layers;
}

auto xml_engine::properties(const object_type& object)
    -> std::vector<object_type>
{
  const auto top = object->elementsByTagName(TACTILE_QSTRING(u"properties"));

  const auto count = top.count();
  if (count == 0) {
    return std::vector<object_type>{};
  }

  const auto topNode = top.at(0);
  Q_ASSERT(topNode.isElement());

  return collect(object_type{topNode.toElement()},
                 TACTILE_QSTRING(u"property"));
}

auto xml_engine::root(const document_type& document) -> object_type
{
  return object_type{document.documentElement()};
}

auto xml_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return xml::from_file(path);
}

auto xml_engine::add_tiles(tile_layer_data& layer,
                           const xml_engine::object_type& element,
                           parse_error& error) -> bool
{
  const auto data = element->firstChildElement(TACTILE_QSTRING(u"data"));
  const auto tiles = data.text().split(u',');

  layer.tiles.reserve(layer.nRows.get());
  layer.tiles.assign(layer.nRows.get(), core::make_tile_row(layer.nCols));

  int index{0};
  for (const auto& value : tiles) {
    bool ok{};
    const tile_id id{value.toInt(&ok)};

    if (!ok) {
      error = parse::parse_error::layer_could_not_parse_tile;
      return false;
    }

    const auto pos = index_to_position(index, layer.nCols);
    layer.tiles.at(pos.row_index()).at(pos.col_index()) = id;

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

auto xml_engine::assume_string_property(const object_type& object) -> bool
{
  return !object->hasAttribute(TACTILE_QSTRING(u"type"));
}

auto xml_engine::is_tile_layer(const object_type& object) -> bool
{
  return object->tagName() == TACTILE_QSTRING(u"layer");
}

auto xml_engine::is_object_layer(const object_type& object) -> bool
{
  return object->tagName() == TACTILE_QSTRING(u"objectgroup");
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

}  // namespace tactile::parse
