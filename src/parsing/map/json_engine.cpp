#include "json_engine.hpp"

#include <utility>  // move

#include "index_to_position.hpp"
#include "ints.hpp"
#include "json_utils.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "to_property.hpp"

namespace tactile {

static_assert(IsParserObject<JsonElement>);
static_assert(IsParserEngine<JsonEngine, QJsonDocument, JsonElement>);

auto JsonEngine::Root(const document_type& document) -> object_type
{
  return object_type{document.object()};
}

auto JsonEngine::FromFile(const QFileInfo& path) -> Maybe<document_type>
{
  return ReadJson(path);
}

auto JsonEngine::Tilesets(const object_type& root) -> std::vector<object_type>
{
  return Collect(root, u"tilesets");
}

auto JsonEngine::Layers(const object_type& root) -> std::vector<object_type>
{
  return Collect(root, u"layers");
}

auto JsonEngine::Properties(const object_type& object)
    -> std::vector<object_type>
{
  return Collect(object, u"properties");
}

auto JsonEngine::Objects(const object_type& object) -> std::vector<object_type>
{
  return Collect(object, u"objects");
}

auto JsonEngine::Tiles(const object_type& object,
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
      const auto pos = IndexToPosition(index, nCols);
      matrix.at(pos.RowIndex()).at(pos.ColumnIndex()) = id;
    }
    else
    {
      error = ParseError::LayerCouldNotParseTile;
      return matrix;
    }

    ++index;
  }

  return matrix;
}

auto JsonEngine::PropertyType(const object_type& object) -> QString
{
  return object.String(ElementId::Type).value();
}

auto JsonEngine::ContainsTilesets(const object_type& object) -> bool
{
  return object.Contains(ElementId::Tilesets);
}

auto JsonEngine::TilesetImageRelativePath(const object_type& object)
    -> Maybe<QString>
{
  return object.String(ElementId::Image);
}

auto JsonEngine::ValidateLayerType(const object_type& object) -> bool
{
  return object->contains(u"type");
}

auto JsonEngine::ContainsLayers(const object_type& object) -> bool
{
  return object->contains(u"layers");
}

auto JsonEngine::IsTileLayer(const object_type& object) -> bool
{
  return object->value(u"type").toString() == TACTILE_QSTRING(u"tilelayer");
}

auto JsonEngine::IsObjectLayer(const object_type& object) -> bool
{
  return object->value(u"type").toString() == TACTILE_QSTRING(u"objectgroup");
}

auto JsonEngine::IsPoint(const object_type& object) -> bool
{
  return object.Boolean(ElementId::Point).value_or(false);
}

auto JsonEngine::Collect(const object_type& root, const QStringView key)
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

}  // namespace tactile
