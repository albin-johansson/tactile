#include "xml_engine.hpp"

#include <cstddef>   // size_t
#include <iterator>  // make_move_iterator

#include "index_to_position.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "to_property.hpp"

namespace tactile::parse {

static_assert(is_object<xml_element>);
static_assert(is_engine<xml_engine, QDomDocument, xml_element>);

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
  if (count == 0)
  {
    return std::vector<object_type>{};
  }

  const auto topNode = top.at(0);
  Q_ASSERT(topNode.isElement());

  return collect(object_type{topNode.toElement()},
                 TACTILE_QSTRING(u"property"));
}

auto xml_engine::objects(const object_type& object) -> std::vector<object_type>
{
  return collect(object, TACTILE_QSTRING(u"object"));
}

auto xml_engine::tiles(const object_type& object,
                       const row_t nRows,
                       const col_t nCols,
                       parse_error& error) -> core::tile_matrix
{
  auto matrix = core::make_tile_matrix(nRows, nCols);

  const auto data = object->firstChildElement(TACTILE_QSTRING(u"data"));
  const auto tiles = data.text().split(u',');

  int index{0};
  for (const auto& value : tiles)
  {
    bool ok{};
    const tile_id id{value.toInt(&ok)};

    if (!ok)
    {
      error = parse::parse_error::layer_could_not_parse_tile;
      return matrix;
    }

    const auto pos = index_to_position(index, nCols);
    matrix.at(pos.row_index()).at(pos.col_index()) = id;

    ++index;
  }

  return matrix;
}

auto xml_engine::property_type(const object_type& object) -> QString
{
  // The following is a quirk due to the fact that the type attribute can be
  // omitted for string properties
  if (assume_string_property(object))
  {
    return TACTILE_QSTRING(u"string");
  }
  else
  {
    return object.string(element_id::type).value();
  }
}

auto xml_engine::root(const document_type& document) -> object_type
{
  return object_type{document.documentElement()};
}

auto xml_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return xml::from_file(path);
}

auto xml_engine::tileset_image_relative_path(const object_type& object)
    -> maybe<QString>
{
  const auto imageElem = object->firstChildElement(TACTILE_QSTRING(u"image"));
  const auto path = imageElem.attribute(TACTILE_QSTRING(u"source"));
  if (!path.isNull())
  {
    return path;
  }
  else
  {
    return std::nullopt;
  }
}

auto xml_engine::contains_tilesets(const object_type& object) -> bool
{
  return true;
}

auto xml_engine::validate_layer_type(const object_type& object) -> bool
{
  return true;
}

auto xml_engine::contains_layers(const object_type& object) -> bool
{
  return true;
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

auto xml_engine::is_point(const object_type& object) -> bool
{
  return has_child(object, TACTILE_QSTRING(u"point"));
}

auto xml_engine::collect(const object_type& root, const QString& key)
    -> std::vector<object_type>
{
  const auto elements = root->elementsByTagName(key);
  const auto count = elements.count();

  std::vector<object_type> result;
  result.reserve(static_cast<std::size_t>(count));

  for (auto i = 0; i < count; ++i)
  {
    const auto& node = elements.at(i);

    Q_ASSERT(node.isElement());
    result.emplace_back(node.toElement());
  }

  return result;
}

auto xml_engine::has_child(const object_type& obj, const QString& tag) -> bool
{
  return !obj->firstChildElement(tag).isNull();
}

}  // namespace tactile::parse
