#include "json_engine.hpp"

#include <cstddef>  // size_t

#include "index_to_position.hpp"
#include "json_utils.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "to_property.hpp"

namespace tactile::parse {

auto json_engine::root(const document_type& document) -> object_type
{
  return object_type{document.object()};
}

auto json_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return json::from_file(path);
}

auto json_engine::tilesets(const object_type& root) -> std::vector<object_type>
{
  return collect(root, u"tilesets");
}

auto json_engine::layers(const object_type& root) -> std::vector<object_type>
{
  return collect(root, u"layers");
}

auto json_engine::properties(const object_type& object)
    -> std::vector<object_type>
{
  return collect(object, u"properties");
}

auto json_engine::add_tiles(tile_layer_data& layer,
                            const object_type& element,
                            parse_error& error) -> bool
{
  const auto data = element->value(u"data").toArray();

  layer.tiles.reserve(layer.nRows.get());
  layer.tiles.assign(layer.nRows.get(), core::make_tile_row(layer.nCols));

  int index{0};
  for (const auto value : data) {
    const tile_id id{value.toInt(-1)};

    if (id != -1_t) {
      const auto pos = index_to_position(index, layer.nCols);
      layer.tiles.at(pos.row_index()).at(pos.col_index()) = id;

    } else {
      error = parse_error::layer_could_not_parse_tile;
      return false;
    }

    ++index;
  }

  return true;
}

auto json_engine::add_objects(object_layer_data& layer,
                              const json_engine::object_type& element,
                              parse_error& error) -> bool
{
  const auto data = element->value(u"objects").toArray();
  const auto emptyString = TACTILE_QSTRING(u"");

  layer.objects.reserve(data.size());

  for (const auto& value : data) {
    Q_ASSERT(value.isObject());
    const object_type elem{value.toObject()};

    auto& object = layer.objects.emplace_back();

    if (const auto id = elem.integer(element_id::id)) {
      object.id = object_id{*id};
    } else {
      error = parse_error::object_missing_id;
      return false;
    }

    object.x = elem.floating(element_id::x, 0);
    object.y = elem.floating(element_id::y, 0);
    object.width = elem.floating(element_id::width, 0);
    object.height = elem.floating(element_id::height, 0);
    object.name = elem.string(element_id::name, emptyString);
    object.customType = elem.string(element_id::type, emptyString);
    object.visible = elem.boolean(element_id::visible).value_or(true);
    object.isPoint = elem.boolean(element_id::point).value_or(false);

    for (const auto& p : properties(elem)) {
      if (auto prop = parse_property(p, error)) {
        object.properties.emplace_back(*prop);
      } else {
        return false;
      }
    }
  }

  return true;
}

auto json_engine::parse_property(const object_type& prop, parse_error& error)
    -> maybe<property_data>
{
  property_data data;
  data.name = prop.string(element_id::name).value();

  const auto type = prop.string(element_id::type).value();

  if (auto p = to_property(prop, type, error)) {
    data.property = std::move(*p);
    return data;
  } else {
    return std::nullopt;
  }
}

auto json_engine::contains_tilesets(const object_type& object) -> bool
{
  return object.contains(element_id::tilesets);
}

auto json_engine::tileset_image_relative_path(const object_type& object)
    -> maybe<QString>
{
  return object.string(element_id::image);
}

auto json_engine::validate_layer_type(const object_type& object) -> bool
{
  return object->contains(u"type");
}

auto json_engine::contains_layers(const object_type& object) -> bool
{
  return object->contains(u"layers");
}

auto json_engine::is_tile_layer(const object_type& object) -> bool
{
  return object->value(u"type").toString() == TACTILE_QSTRING(u"tilelayer");
}

auto json_engine::is_object_layer(const object_type& object) -> bool
{
  return object->value(u"type").toString() == TACTILE_QSTRING(u"objectgroup");
}

auto json_engine::collect(const object_type& root, const QStringView key)
    -> std::vector<object_type>
{
  const auto array = root->value(key).toArray();

  std::vector<object_type> vector;
  vector.reserve(static_cast<std::size_t>(array.size()));

  for (const auto& elem : array) {
    vector.emplace_back(elem.toObject());
  }

  return vector;
}

}  // namespace tactile::parse
