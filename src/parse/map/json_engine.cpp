#include "json_engine.hpp"

#include <cstddef>  // size_t

#include "json_utils.hpp"
#include "tactile_qstring.hpp"

namespace tactile::tmx {
namespace {

// TODO move this elsewhere?
[[nodiscard]] auto index_to_position(const int index, const int nCols)
    -> core::position
{
  return core::position{row_t{index / nCols}, col_t{index % nCols}};
}

}  // namespace

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

auto json_engine::add_tiles(core::tile_layer& layer,
                            const object_type& element,
                            parse_error& error) -> bool
{
  const auto nCols = layer.col_count().get();
  const auto data = element->value(u"data").toArray();

  int index{0};
  for (const auto value : data) {
    const tile_id id{value.toInt(-1)};

    if (id == -1_t) {
      error = parse_error::layer_could_not_parse_tile;
      return false;
    } else {
      layer.set_tile(index_to_position(index, nCols), id);
    }

    ++index;
  }

  return true;
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

auto json_engine::assume_string_property(const object_type& object) -> bool
{
  return false;
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

}  // namespace tactile::map
