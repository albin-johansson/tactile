#include "to_map_document.hpp"

#include <utility>  // move

#include "object_layer.hpp"
#include "tile_layer.hpp"

namespace tactile::parse {
namespace {

[[nodiscard]] auto make_tileset(const tileset_data& data)
    -> shared<core::tileset>
{
  auto tileset = std::make_shared<core::tileset>(data.firstId,
                                                 data.absolutePath,
                                                 data.tileWidth,
                                                 data.tileHeight);
  tileset->set_name(data.name);
  return tileset;
}

[[nodiscard]] auto make_tile_layer(const tile_layer_data& data)
    -> shared<core::layer>
{
  auto layer = std::make_shared<core::tile_layer>(data.nRows, data.nCols);

  for (auto row = 0_row; row < data.nRows; ++row) {
    for (auto col = 0_col; col < data.nCols; ++col) {
      const core::position pos{row, col};
      layer->set_tile(pos, data.tiles.at(pos.row_index()).at(pos.col_index()));
    }
  }

  return layer;
}

[[nodiscard]] auto make_object(const object_data& objectData) -> core::object
{
  const auto getType = [](const object_data& objectData) {
    if (objectData.isPoint) {
      return core::object_type::point;
    } else {
      return core::object_type::rectangle;
    }
  };

  core::object object{getType(objectData)};

  object.set_x(objectData.x);
  object.set_y(objectData.y);
  object.set_width(objectData.width);
  object.set_height(objectData.height);
  object.set_name(objectData.name);
  object.set_visible(objectData.visible);

  for (const auto& propertyData : objectData.properties) {
    object.add_property(propertyData.name, propertyData.property);
  }

  return object;
}

[[nodiscard]] auto make_object_layer(const object_layer_data& data)
    -> shared<core::layer>
{
  auto layer = std::make_shared<core::object_layer>();

  for (const auto& objectData : data.objects) {
    layer->add_object(objectData.id, make_object(objectData));
  }

  return layer;
}

[[nodiscard]] auto make_layer(const layer_data& data) -> shared<core::layer>
{
  shared<core::layer> layer;

  if (data.type == core::layer_type::tile_layer) {
    layer = make_tile_layer(std::get<tile_layer_data>(data.data));

  } else if (data.type == core::layer_type::object_layer) {
    layer = make_object_layer(std::get<object_layer_data>(data.data));
  }

  Q_ASSERT(layer);
  layer->set_name(data.name);
  layer->set_opacity(data.opacity);
  layer->set_visible(data.visible);

  for (const auto& propertyData : data.properties) {
    layer->add_property(propertyData.name, propertyData.property);
  }

  return layer;
}

}  // namespace

auto to_map_document(const map_data& data) -> core::map_document*
{
  Q_ASSERT(!data.layers.empty());

  auto* document = new core::map_document{};

  document->set_next_layer_id(data.nextLayerId);
  document->set_next_object_id(data.nextObjectId);

  for (const auto& tilesetData : data.tilesets) {
    document->add_tileset(make_tileset(tilesetData));
  }

  for (const auto& layerData : data.layers) {
    document->add_layer(layerData.id, make_layer(layerData));
  }

  for (const auto& propertyData : data.properties) {
    document->add_property(propertyData.name, propertyData.property);
  }

  document->select_layer(data.layers.front().id);
  document->reset_history();  // Make sure there is no undo/redo history

  return document;
}

}  // namespace tactile::parse
