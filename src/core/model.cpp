#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model() : m_maps{new map_manager{this}}, m_tools{this}
{
  // clang-format off
  connect(m_maps, &map_manager::undo_state_updated, this, &model::undo_state_updated);
  connect(m_maps, &map_manager::redo_state_updated, this, &model::redo_state_updated);
  connect(m_maps, &map_manager::undo_text_updated, this, &model::undo_text_updated);
  connect(m_maps, &map_manager::redo_text_updated, this, &model::redo_text_updated);
  connect(m_maps, &map_manager::added_layer, this, &model::added_layer);
  connect(m_maps, &map_manager::removed_layer, this, &model::removed_layer);
  connect(m_maps, &map_manager::selected_layer, this, &model::selected_layer);
  connect(m_maps, &map_manager::moved_layer_back, this, &model::moved_layer_back);
  connect(m_maps, &map_manager::moved_layer_forward, this, &model::moved_layer_forward);
  connect(m_maps, &map_manager::removed_tileset, this, &model::removed_tileset);
  connect(m_maps, &map_manager::added_tileset, [this](tileset_id id) {
    const auto& tileset = current_document()->tilesets()->at(id);
    emit added_tileset(current_map_id().value(), id, tileset);
  });
  // clang-format on
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

void model::add_layer()
{
  if (auto* document = current_document()) {
    document->add_layer();
  }
}

void model::remove_active_layer()
{
  if (auto* document = current_document()) {
    document->remove_active_layer();
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

void model::ui_selected_map(map_id id)
{
  m_maps->select(id);

  auto* document = m_maps->at(id);
  Q_ASSERT(document);

  emit switched_map(id, *document);
}

}  // namespace tactile::core
