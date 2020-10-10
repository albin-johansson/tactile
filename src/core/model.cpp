#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model() : m_maps{std::make_unique<map_manager>()}, m_tools{this}
{
  connect(m_maps.get(),
          &map_manager::undo_state_updated,
          this,
          &model::undo_state_updated);
  connect(m_maps.get(),
          &map_manager::redo_state_updated,
          this,
          &model::redo_state_updated);
  connect(m_maps.get(),
          &map_manager::undo_text_updated,
          this,
          &model::undo_text_updated);
  connect(m_maps.get(),
          &map_manager::redo_text_updated,
          this,
          &model::redo_text_updated);
}

void model::undo()
{
  if (auto* document = current_document()) {
    document->undo();
    emit redraw();
  }
}

void model::redo()
{
  if (auto* document = current_document()) {
    document->redo();
    emit redraw();
  }
}

void model::resize_map(row_t nRows, col_t nCols)
{
  if (auto* document = current_document()) {
    document->resize(nRows, nCols);
    emit redraw();
  }
}

void model::add_row()
{
  if (auto* document = current_document()) {
    document->add_row();
    emit redraw();
  }
}

void model::add_col()
{
  if (auto* document = current_document()) {
    document->add_column();
    emit redraw();
  }
}

void model::remove_row()
{
  if (auto* document = current_document()) {
    document->remove_row();
    emit redraw();
  }
}

void model::remove_col()
{
  if (auto* document = current_document()) {
    document->remove_column();
    emit redraw();
  }
}

void model::select_layer(layer_id id)
{
  if (auto* document = current_document()) {
    document->select_layer(id);
    emit redraw();
  }
}

void model::increase_tile_size()
{
  if (auto* document = current_document()) {
    document->increase_tile_size();
    emit redraw();
  }
}

void model::decrease_tile_size()
{
  if (auto* document = current_document()) {
    document->decrease_tile_size();
    emit redraw();
  }
}

void model::reset_tile_size()
{
  if (auto* document = current_document()) {
    document->reset_tile_size();
    emit redraw();
  }
}

void model::select_tool(tool_id tool)
{
  m_tools.select(tool);
}

void model::select_tileset(tileset_id id)
{
  m_maps->select_tileset(id);
}

auto model::add_map() -> map_id
{
  return m_maps->add();
}

void model::close_map(map_id id)
{
  m_maps->close(id);
}

void model::select_map(map_id id)
{
  m_maps->select(id);
  emit switched_map(id);
}

auto model::add_tileset(const QImage& image,
                        const QString& path,
                        const QString& name,
                        tile_width tileWidth,
                        tile_height tileHeight) -> std::optional<tileset_id>
{
  return m_maps->add_tileset(image, path, name, tileWidth, tileHeight);
}

void model::remove_tileset(tileset_id id)
{
  m_maps->remove_tileset(id);
  emit redraw();
}

void model::update_tileset_selection(position topLeft, position bottomRight)
{
  m_maps->update_tileset_selection(topLeft, bottomRight);
}

}  // namespace tactile::core
