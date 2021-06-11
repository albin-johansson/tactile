#include "json_engine.hpp"

#include <utility>  // move

#include "index_to_position.hpp"
#include "ints.hpp"
#include "json_utils.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "to_property.hpp"

namespace tactile::parse {

static_assert(is_object<json_element>);
static_assert(is_engine<json_engine, QJsonDocument, json_element>);

auto json_engine::root(const document_type& document) -> object_type
{
  return object_type{document.object()};
}

auto json_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return ReadJson(path);
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

auto json_engine::objects(const object_type& object) -> std::vector<object_type>
{
  return collect(object, u"objects");
}

auto json_engine::tiles(const object_type& object,
                        const row_t nRows,
                        const col_t nCols,
                        parse_error& error) -> core::tile_matrix
{
  auto matrix = core::MakeTileMatrix(nRows, nCols);

  const auto data = object->value(u"data").toArray();
  int index{0};

  for (const auto& value : data)
  {
    const tile_id id{value.toInt(-1)};

    if (id != -1_t)
    {
      const auto pos = index_to_position(index, nCols);
      matrix.at(pos.RowIndex()).at(pos.ColumnIndex()) = id;
    }
    else
    {
      error = parse_error::layer_could_not_parse_tile;
      return matrix;
    }

    ++index;
  }

  return matrix;
}

auto json_engine::property_type(const object_type& object) -> QString
{
  return object.string(element_id::type).value();
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

auto json_engine::is_point(const object_type& object) -> bool
{
  return object.boolean(element_id::point).value_or(false);
}

auto json_engine::collect(const object_type& root, const QStringView key)
    -> std::vector<object_type>
{
  const auto array = root->value(key).toArray();

  std::vector<object_type> vector;
  vector.reserve(static_cast<usize>(array.size()));

  for (const auto& elem : array)
  {
    Q_ASSERT(elem.isObject());
    vector.emplace_back(elem.toObject());
  }

  return vector;
}

}  // namespace tactile::parse
