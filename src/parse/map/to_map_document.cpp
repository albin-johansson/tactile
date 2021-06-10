#include "to_map_document.hpp"

#include <utility>  // move

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::parse {
namespace {

[[nodiscard]] auto make_tileset(const tileset_data& data)
    -> shared<core::Tileset>
{
  auto tileset = std::make_shared<core::Tileset>(data.firstId,
                                                 data.absolutePath,
                                                 data.tileWidth,
                                                 data.tileHeight);
  tileset->SetName(data.name);
  return tileset;
}

[[nodiscard]] auto make_tile_layer(const tile_layer_data& data)
    -> shared<core::ILayer>
{
  auto layer = std::make_shared<core::TileLayer>(data.nRows, data.nCols);

  for (auto row = 0_row; row < data.nRows; ++row)
  {
    for (auto col = 0_col; col < data.nCols; ++col)
    {
      const core::Position pos{row, col};
      layer->SetTile(pos, data.tiles.at(pos.RowIndex()).at(pos.ColumnIndex()));
    }
  }

  return layer;
}

[[nodiscard]] auto make_object(const object_data& objectData) -> core::Object
{
  const auto getType = [](const object_data& objectData) {
    if (objectData.isPoint)
    {
      return core::object_type::point;
    }
    else
    {
      return core::object_type::rectangle;
    }
  };

  core::Object object{getType(objectData)};

  object.SetX(objectData.x);
  object.SetY(objectData.y);
  object.SetWidth(objectData.width);
  object.SetHeight(objectData.height);
  object.SetName(objectData.name);
  object.SetVisible(objectData.visible);

  for (const auto& propertyData : objectData.properties)
  {
    object.AddProperty(propertyData.name, propertyData.property);
  }

  return object;
}

[[nodiscard]] auto make_object_layer(const object_layer_data& data)
    -> shared<core::ILayer>
{
  auto layer = std::make_shared<core::ObjectLayer>();

  for (const auto& objectData : data.objects)
  {
    layer->AddObject(objectData.id, make_object(objectData));
  }

  return layer;
}

[[nodiscard]] auto make_layer(const layer_data& data) -> shared<core::ILayer>
{
  shared<core::ILayer> layer;

  if (data.type == core::LayerType::tile_layer)
  {
    layer = make_tile_layer(std::get<tile_layer_data>(data.data));
  }
  else if (data.type == core::LayerType::object_layer)
  {
    layer = make_object_layer(std::get<object_layer_data>(data.data));
  }

  Q_ASSERT(layer);
  layer->SetName(data.name);
  layer->SetOpacity(data.opacity);
  layer->SetVisible(data.visible);

  for (const auto& propertyData : data.properties)
  {
    layer->AddProperty(propertyData.name, propertyData.property);
  }

  return layer;
}

}  // namespace

auto to_map_document(const map_data& data) -> core::MapDocument*
{
  Q_ASSERT(!data.layers.empty());

  auto* document = new core::MapDocument{};

  document->SetNextLayerId(data.nextLayerId);
  document->SetNextObjectId(data.nextObjectId);

  auto* tilesets = document->GetTilesets();
  for (const auto& tilesetData : data.tilesets)
  {
    const auto id [[maybe_unused]] = tilesets->add(make_tileset(tilesetData));
  }

  for (const auto& layerData : data.layers)
  {
    document->AddLayer(layerData.id, make_layer(layerData));
  }

  for (const auto& propertyData : data.properties)
  {
    document->AddProperty(propertyData.name, propertyData.property);
  }

  document->SetPath(QFileInfo{data.path});
  document->SelectLayer(data.layers.front().id);
  document->ResetHistory();  // Make sure there is no undo/redo history

  return document;
}

}  // namespace tactile::parse
