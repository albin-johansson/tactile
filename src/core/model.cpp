#include "model.hpp"

#include "map_manager.hpp"

namespace tactile::core {

model::model() : m_maps{new map_manager{this}}, m_tools{this}
{
  // clang-format off
  connect(m_maps, &map_manager::undo_state_updated, this, &model::undo_state_updated);
  connect(m_maps, &map_manager::redo_state_updated, this, &model::redo_state_updated);
  connect(m_maps, &map_manager::undo_text_updated, this, &model::undo_text_updated);
  connect(m_maps, &map_manager::redo_text_updated, this, &model::redo_text_updated);
  connect(m_maps, &map_manager::added_layer, this, &model::added_layer);
  connect(m_maps, &map_manager::added_duplicated_layer, this, &model::added_duplicated_layer);
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

auto model::add_map() -> map_id
{
  return m_maps->add();
}

auto model::add_map(map_document* document) -> map_id
{
  return m_maps->add(document);
}

auto model::has_active_map() const noexcept -> bool
{
  return m_maps->has_active_map();
}

auto model::get_document(map_id id) -> map_document*
{
  return m_maps->at(id);
}

auto model::current_map_id() const -> std::optional<map_id>
{
  return m_maps->current_map_id();
}

auto model::current_document() -> map_document*
{
  return m_maps->current_document();
}

auto model::current_document() const -> const map_document*
{
  return m_maps->current_document();
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

void model::remove_layer(layer_id id)
{
  if (auto* document = current_document()) {
    document->remove_layer(id);
  }
}

void model::select_layer(layer_id id)
{
  if (auto* document = current_document()) {
    document->select_layer(id);
    emit redraw();
  }
}

void model::select_tool(tool_id id)
{
  m_tools.select(id);
}

void model::select_tileset(tileset_id id)
{
  if (auto* document = current_document()) {
    document->select_tileset(id);
  }
}

void model::set_tileset_selection(const tileset::selection& selection)
{
  if (auto* document = current_document()) {
    document->set_tileset_selection(selection);
  }
}

void model::set_layer_visibility(layer_id id, bool visible)
{
  if (auto* document = current_document()) {
    document->set_layer_visibility(id, visible);
    emit redraw();
  }
}

void model::set_layer_opacity(layer_id id, double opacity)
{
  if (auto* document = current_document()) {
    document->set_layer_opacity(id, opacity);
    emit redraw();
  }
}

void model::set_layer_name(layer_id id, const QString& name)
{
  if (auto* document = current_document()) {
    document->set_layer_name(id, name);
    emit redraw();
  }
}

void model::move_layer_back(layer_id id)
{
  if (auto* document = current_document()) {
    document->move_layer_back(id);
    emit redraw();
  }
}

void model::move_layer_forward(layer_id id)
{
  if (auto* document = current_document()) {
    document->move_layer_forward(id);
    emit redraw();
  }
}

void model::duplicate_layer(layer_id id)
{
  if (auto* document = current_document()) {
    document->duplicate_layer(id);
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

void model::create_tileset(const QImage& image,
                           const QFileInfo& path,
                           const QString& name,
                           tile_width tileWidth,
                           tile_height tileHeight)
{
  if (auto* document = current_document()) {
    document->add_tileset(image, path, name, tileWidth, tileHeight);
  }
}

void model::remove_tileset(tileset_id id)
{
  if (auto* document = current_document()) {
    document->ui_remove_tileset(id);
    emit redraw();
  }
}

void model::set_tileset_name(tileset_id id, const QString& name)
{
  if (auto* document = current_document()) {
    document->set_tileset_name(id, name);
  }
}

void model::select_map(map_id id)
{
  m_maps->select(id);

  auto* document = m_maps->current_document();
  Q_ASSERT(document);

  emit switched_map(id, *document);
}

void model::close_map(map_id id)
{
  m_maps->close(id);
}

void model::mouse_pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  m_tools.pressed(event, mapPosition);
}

void model::mouse_moved(QMouseEvent* event, const QPointF& mapPosition)
{
  m_tools.moved(event, mapPosition);
}

void model::mouse_released(QMouseEvent* event, const QPointF& mapPosition)
{
  m_tools.released(event, mapPosition);
}

void model::mouse_entered(QEvent* event)
{
  m_tools.entered(event);
}

void model::mouse_exited(QEvent* event)
{
  m_tools.exited(event);
}

}  // namespace tactile::core
