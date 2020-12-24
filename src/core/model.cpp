#include "model.hpp"

#include "map_document_manager.hpp"

namespace tactile::core {

model::model() : m_mapDocuments{new map_document_manager{this}}, m_tools{this}
{
  // clang-format off
  connect(m_mapDocuments, &map_document_manager::undo_state_updated,     this, &model::undo_state_updated);
  connect(m_mapDocuments, &map_document_manager::redo_state_updated,     this, &model::redo_state_updated);
  connect(m_mapDocuments, &map_document_manager::undo_text_updated,      this, &model::undo_text_updated);
  connect(m_mapDocuments, &map_document_manager::redo_text_updated,      this, &model::redo_text_updated);
  connect(m_mapDocuments, &map_document_manager::clean_changed,          this, &model::clean_changed);
  connect(m_mapDocuments, &map_document_manager::added_layer,            this, &model::added_layer);
  connect(m_mapDocuments, &map_document_manager::added_duplicated_layer, this, &model::added_duplicated_layer);
  connect(m_mapDocuments, &map_document_manager::removed_layer,          this, &model::removed_layer);
  connect(m_mapDocuments, &map_document_manager::selected_layer,         this, &model::selected_layer);
  connect(m_mapDocuments, &map_document_manager::moved_layer_back,       this, &model::moved_layer_back);
  connect(m_mapDocuments, &map_document_manager::moved_layer_forward,    this, &model::moved_layer_forward);
  connect(m_mapDocuments, &map_document_manager::removed_tileset,        this, &model::removed_tileset);
  connect(m_mapDocuments, &map_document_manager::added_property,         this, &model::added_property);
  connect(m_mapDocuments, &map_document_manager::removed_property,       this, &model::removed_property);
  connect(m_mapDocuments, &map_document_manager::moved_property_up,      this, &model::moved_property_up);
  connect(m_mapDocuments, &map_document_manager::moved_property_down,    this, &model::moved_property_down);
  connect(m_mapDocuments, &map_document_manager::duplicated_property,    this, &model::duplicated_property);

  connect(m_mapDocuments, &map_document_manager::added_tileset, [this](tileset_id id) {
    const auto& tileset = current_document()->tilesets()->at(id);
    emit added_tileset(current_map_id().value(), id, tileset);
  });
  // clang-format on
}

auto model::add_map() -> map_id
{
  return m_mapDocuments->add();
}

auto model::add_map(map_document* document) -> map_id
{
  return m_mapDocuments->add(document);
}

auto model::has_active_map() const noexcept -> bool
{
  return m_mapDocuments->has_active_map();
}

auto model::get_document(const map_id id) -> map_document*
{
  return m_mapDocuments->at(id);
}

auto model::current_map_id() const -> maybe<map_id>
{
  return m_mapDocuments->current_map_id();
}

auto model::current_document() -> map_document*
{
  return m_mapDocuments->current_document();
}

auto model::current_document() const -> const map_document*
{
  return m_mapDocuments->current_document();
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

void model::resize_map(const row_t nRows, const col_t nCols)
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

void model::remove_layer(const layer_id id)
{
  if (auto* document = current_document()) {
    document->remove_layer(id);
  }
}

void model::select_layer(const layer_id id)
{
  if (auto* document = current_document()) {
    document->select_layer(id);
    emit redraw();
  }
}

void model::select_tool(const tool_id id)
{
  m_tools.select(id);
}

void model::select_tileset(const tileset_id id)
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

void model::set_layer_visibility(const layer_id id, const bool visible)
{
  if (auto* document = current_document()) {
    document->set_layer_visibility(id, visible);
    emit redraw();
  }
}

void model::set_layer_opacity(const layer_id id, const double opacity)
{
  if (auto* document = current_document()) {
    document->set_layer_opacity(id, opacity);
    emit redraw();
  }
}

void model::set_layer_name(const layer_id id, const QString& name)
{
  if (auto* document = current_document()) {
    document->set_layer_name(id, name);
    emit redraw();
  }
}

void model::move_layer_back(const layer_id id)
{
  if (auto* document = current_document()) {
    document->move_layer_back(id);
    emit redraw();
  }
}

void model::move_layer_forward(const layer_id id)
{
  if (auto* document = current_document()) {
    document->move_layer_forward(id);
    emit redraw();
  }
}

void model::duplicate_layer(const layer_id id)
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
                           const tile_width tileWidth,
                           const tile_height tileHeight)
{
  if (auto* document = current_document()) {
    document->add_tileset(image, path, name, tileWidth, tileHeight);
  }
}

void model::remove_tileset(const tileset_id id)
{
  if (auto* document = current_document()) {
    document->ui_remove_tileset(id);
    emit redraw();
  }
}

void model::set_tileset_name(const tileset_id id, const QString& name)
{
  if (auto* document = current_document()) {
    document->set_tileset_name(id, name);
  }
}

void model::add_property(const QString& name, const core::property::type type)
{
  if (auto* document = current_document()) {
    document->add_property(name, type);
  }
}

void model::remove_property(const QString& name)
{
  if (auto* document = current_document()) {
    document->remove_property(name);
  }
}

void model::rename_property(const QString& oldName, const QString& newName)
{
  if (auto* document = current_document()) {
    document->rename_property(oldName, newName);
  }
}

void model::set_property(const QString& name, const core::property& property)
{
  if (auto* document = current_document()) {
    document->set_property(name, property);
  }
}

void model::select_map(const map_id id)
{
  m_mapDocuments->select(id);

  auto* document = m_mapDocuments->current_document();
  Q_ASSERT(document);

  emit switched_map(id, *document);
}

void model::close_map(const map_id id)
{
  m_mapDocuments->close(id);
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
