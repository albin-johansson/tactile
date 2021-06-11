#include "to_map_document.hpp"

#include <utility>  // move

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::parse {
namespace {

[[nodiscard]] auto MakeTileset(const TilesetData& data) -> shared<core::Tileset>
{
  auto tileset = std::make_shared<core::Tileset>(data.firstId,
                                                 data.absolutePath,
                                                 data.tileWidth,
                                                 data.tileHeight);
  tileset->SetName(data.name);
  return tileset;
}

[[nodiscard]] auto MakeTileLayer(const TileLayerData& data)
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

[[nodiscard]] auto MakeObject(const ObjectData& objectData) -> core::Object
{
  const auto getType = [](const ObjectData& objectData) {
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

[[nodiscard]] auto MakeObjectLayer(const ObjectLayerData& data)
    -> shared<core::ILayer>
{
  auto layer = std::make_shared<core::ObjectLayer>();

  for (const auto& objectData : data.objects)
  {
    layer->AddObject(objectData.id, MakeObject(objectData));
  }

  return layer;
}

[[nodiscard]] auto MakeLayer(const LayerData& data) -> shared<core::ILayer>
{
  shared<core::ILayer> layer;

  if (data.type == core::LayerType::tile_layer)
  {
    layer = MakeTileLayer(std::get<TileLayerData>(data.data));
  }
  else if (data.type == core::LayerType::object_layer)
  {
    layer = MakeObjectLayer(std::get<ObjectLayerData>(data.data));
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

auto ToMapDocument(const MapData& data) -> core::MapDocument*
{
  Q_ASSERT(!data.layers.empty());

  auto* document = new core::MapDocument{};

  document->SetNextLayerId(data.nextLayerId);
  document->SetNextObjectId(data.nextObjectId);

  auto* tilesets = document->GetTilesets();
  for (const auto& tilesetData : data.tilesets)
  {
    const auto id [[maybe_unused]] = tilesets->Add(MakeTileset(tilesetData));
  }

  for (const auto& layerData : data.layers)
  {
    document->AddLayer(layerData.id, MakeLayer(layerData));
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
