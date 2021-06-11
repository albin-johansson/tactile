#include "json_engine.hpp"

#include <utility>  // move

#include "index_to_position.hpp"
#include "ints.hpp"
#include "json_utils.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "to_property.hpp"

namespace tactile::parse {

static_assert(IsObject<JsonElement>);
static_assert(IsEngine<json_engine, QJsonDocument, JsonElement>);

auto json_engine::Root(const document_type& document) -> object_type
{
  return object_type{document.object()};
}

auto json_engine::FromFile(const QFileInfo& path) -> maybe<document_type>
{
  return ReadJson(path);
}

auto json_engine::Tilesets(const object_type& root) -> std::vector<object_type>
{
  return Collect(root, u"tilesets");
}

auto json_engine::Layers(const object_type& root) -> std::vector<object_type>
{
  return Collect(root, u"layers");
}

auto json_engine::Properties(const object_type& object)
    -> std::vector<object_type>
{
  return Collect(object, u"properties");
}

auto json_engine::Objects(const object_type& object) -> std::vector<object_type>
{
  return Collect(object, u"objects");
}

auto json_engine::Tiles(const object_type& object,
                        const row_t nRows,
                        const col_t nCols,
                        ParseError& error) -> core::tile_matrix
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
      error = ParseError::layer_could_not_parse_tile;
      return matrix;
    }

    ++index;
  }

  return matrix;
}

auto json_engine::PropertyType(const object_type& object) -> QString
{
  return object.String(ElementId::Type).value();
}

auto json_engine::ContainsTilesets(const object_type& object) -> bool
{
  return object.Contains(ElementId::Tilesets);
}

auto json_engine::TilesetImageRelativePath(const object_type& object)
    -> maybe<QString>
{
  return object.String(ElementId::Image);
}

auto json_engine::ValidateLayerType(const object_type& object) -> bool
{
  return object->contains(u"type");
}

auto json_engine::ContainsLayers(const object_type& object) -> bool
{
  return object->contains(u"layers");
}

auto json_engine::IsTileLayer(const object_type& object) -> bool
{
  return object->value(u"type").toString() == TACTILE_QSTRING(u"tilelayer");
}

auto json_engine::IsObjectLayer(const object_type& object) -> bool
{
  return object->value(u"type").toString() == TACTILE_QSTRING(u"objectgroup");
}

auto json_engine::IsPoint(const object_type& object) -> bool
{
  return object.Boolean(ElementId::Point).value_or(false);
}

auto json_engine::Collect(const object_type& root, const QStringView key)
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
