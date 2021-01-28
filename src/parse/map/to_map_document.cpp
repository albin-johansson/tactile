#include "to_map_document.hpp"

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

[[nodiscard]] auto make_object_layer(const object_layer_data& data)
    -> shared<core::layer>
{
  auto layer = std::make_shared<core::object_layer>();

  // TODO

  return layer;
}

}  // namespace

auto to_map_document(const map_data& data) -> core::map_document*
{
  auto* document = new core::map_document{};
  document->set_next_layer_id(data.nextLayerId);
  document->set_next_object_id(data.nextObjectId);

  for (const auto& tilesetData : data.tilesets) {
    document->add_tileset(make_tileset(tilesetData));
  }

  for (const auto& layerData : data.layers) {
    shared<core::layer> layer;

    if (layerData.type == core::layer_type::tile_layer) {
      layer = make_tile_layer(std::get<tile_layer_data>(layerData.data));

    } else if (layerData.type == core::layer_type::object_layer) {
      layer = make_object_layer(std::get<object_layer_data>(layerData.data));
    }

    Q_ASSERT(layer);
    layer->set_name(layerData.name);
    layer->set_opacity(layerData.opacity);
    layer->set_visible(layerData.visible);

    document->add_layer(layerData.id, layer);
  }

  for (const auto& propertyData : data.properties) {
    document->add_property(propertyData.name, propertyData.property);
  }

  document->reset_history();  // Make sure there is no undo/redo history
  return document;
}

}  // namespace tactile::parse
