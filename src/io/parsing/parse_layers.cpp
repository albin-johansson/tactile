#include "parse_layers.hpp"

#include <utility>  // move

#include "core/map/layers/tile_layer.hpp"
#include "io/parsing/parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTileLayer(const IMapObject& object, LayerData& layer)
    -> ParseError
{
  auto& data = layer.data.emplace<TileLayerData>();

  if (const auto rows = object.GetInt(MapAttribute::Height))
  {
    data.row_count = row_t{*rows};
  }
  else
  {
    return ParseError::LayerMissingHeight;
  }

  if (const auto cols = object.GetInt(MapAttribute::Width))
  {
    data.col_count = col_t{*cols};
  }
  else
  {
    return ParseError::LayerMissingWidth;
  }

  data.tiles = MakeTileMatrix(data.row_count, data.col_count);
  if (const auto err = object.GetTileData(data.col_count, data.tiles);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseObjectLayer(const IMapObject& source, LayerData& layer)
    -> ParseError
{
  auto& data = layer.data.emplace<ObjectLayerData>();

  for (const auto& elem : source.GetObjects())
  {
    auto& object = data.objects.emplace_back();

    if (const auto id = elem->GetInt(MapAttribute::Id))
    {
      object.id = object_id{*id};
    }
    else
    {
      return ParseError::ObjectMissingId;
    }

    object.x = elem->GetFloat(MapAttribute::X).value_or(0.0f);
    object.y = elem->GetFloat(MapAttribute::Y).value_or(0.0f);
    object.width = elem->GetFloat(MapAttribute::Width).value_or(0.0f);
    object.height = elem->GetFloat(MapAttribute::Height).value_or(0.0f);
    object.name = elem->GetString(MapAttribute::Name).value_or(std::string{});
    object.custom_type = elem->GetString(MapAttribute::Type).value_or(std::string{});
    object.visible = elem->GetBool(MapAttribute::Visible).value_or(true);

    if (elem->IsPoint())
    {
      object.type = ObjectType::Point;
    }
    else if (elem->IsEllipse())
    {
      object.type = ObjectType::Ellipse;
    }
    else
    {
      object.type = ObjectType::Rectangle;
    }

    if (const auto err = ParseProperties(*elem, object.properties);
        err != ParseError::None)
    {
      return err;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const IMapObject& object, MapData& data) -> ParseError
{
  if (!object.HasType())
  {
    return ParseError::LayerMissingType;
  }

  LayerData layer;

  if (const auto id = object.GetInt(MapAttribute::Id))
  {
    layer.id = layer_id{*id};
  }
  else
  {
    return ParseError::LayerMissingId;
  }

  layer.name = object.GetString(MapAttribute::Name).value_or("Layer");
  layer.opacity = object.GetFloat(MapAttribute::Opacity).value_or(1.0f);
  layer.is_visible = object.GetBool(MapAttribute::Visible).value_or(true);

  if (object.IsTileLayer())
  {
    layer.type = LayerType::TileLayer;
    if (const auto err = ParseTileLayer(object, layer); err != ParseError::None)
    {
      return err;
    }
  }
  else if (object.IsObjectLayer())
  {
    layer.type = LayerType::ObjectLayer;
    if (const auto err = ParseObjectLayer(object, layer); err != ParseError::None)
    {
      return err;
    }
  }
  else
  {
    return ParseError::LayerUnknownType;
  }

  if (const auto err = ParseProperties(object, layer.properties);
      err != ParseError::None)
  {
    return err;
  }

  data.layers.push_back(std::move(layer));

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const IMapFile& file, MapData& data) -> ParseError
{
  if (!file.ContainsLayers())
  {
    return ParseError::MapMissingLayers;
  }

  for (const auto& layer : file.GetLayers())
  {
    if (const auto error = ParseLayer(*layer, data); error != ParseError::None)
    {
      return error;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
