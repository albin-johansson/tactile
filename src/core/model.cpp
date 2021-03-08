#include "model.hpp"

#include "layer_model.hpp"
#include "map_document.hpp"
#include "property_model.hpp"

namespace tactile::core {

model::model() : m_tools{this}
{}

auto model::add_map() -> map_id
{
  return add_map(new map_document{5_row, 5_col, this});
}

auto model::add_map(map_document* document) -> map_id
{
  Q_ASSERT(document);
  Q_ASSERT(!m_documents.contains(m_nextId));

  document->setParent(this);

  const auto bind = [document, this](auto&& signal, auto&& slot) {
    QObject::connect(document, signal, this, slot);
  };

  // clang-format off
  bind(&map_document::redraw,                   &model::redraw);
  bind(&map_document::undo_state_updated,       &model::undo_state_updated);
  bind(&map_document::redo_state_updated,       &model::redo_state_updated);
  bind(&map_document::undo_text_updated,        &model::undo_text_updated);
  bind(&map_document::redo_text_updated,        &model::redo_text_updated);
  bind(&map_document::clean_changed,            &model::clean_changed);
  bind(&map_document::removed_tileset,          &model::removed_tileset);
  bind(&map_document::added_layer,              &model::added_layer);
  bind(&map_document::added_duplicated_layer,   &model::added_duplicated_layer);
  bind(&map_document::removed_layer,            &model::removed_layer);
  bind(&map_document::selected_layer,           &model::selected_layer);
  bind(&map_document::moved_layer_back,         &model::moved_layer_back);
  bind(&map_document::moved_layer_forward,      &model::moved_layer_forward);
  bind(&map_document::added_property,           &model::added_property);
  bind(&map_document::about_to_remove_property, &model::about_to_remove_property);
  bind(&map_document::updated_property,         &model::updated_property);
  bind(&map_document::renamed_property,         &model::renamed_property);
  bind(&map_document::changed_property_type,    &model::changed_property_type);
  // clang-format on

  bind(&map_document::added_tileset, [this](const tileset_id id) {
    const auto& tileset = current_document()->tilesets()->at(id);
    emit added_tileset(current_map_id().value(), id, tileset);
  });

  bind(&map_document::show_properties, [this] {
    emit show_map_properties(current_document());
  });

  bind(&map_document::show_layer_properties, [this](const layer_id id) {
    emit show_layer_properties(current_document()->get_layer(id));
  });

  const auto id = m_nextId;
  m_documents.emplace(id, document);
  m_currentMap = id;

  ++m_nextId;

  emit_undo_redo_update();

  return id;
}

auto model::has_active_map() const noexcept -> bool
{
  return m_currentMap.has_value();
}

auto model::get_document(const map_id id) -> map_document*
{
  return m_documents.at(id);
}

auto model::current_map_id() const -> maybe<map_id>
{
  return m_currentMap;
}

auto model::current_document() -> map_document*
{
  return m_currentMap ? m_documents.at(m_currentMap.value()) : nullptr;
}

auto model::current_document() const -> const map_document*
{
  return m_currentMap ? m_documents.at(m_currentMap.value()) : nullptr;
}

void model::undo()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->undo();
}

void model::redo()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->redo();
}

void model::resize_map(const row_t nRows, const col_t nCols)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->resize(nRows, nCols);
}

void model::add_row()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_row();
}

void model::add_col()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_column();
}

void model::remove_row()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->remove_row();
}

void model::remove_col()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->remove_column();
}

void model::add_layer()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_tile_layer();
}

void model::remove_layer(const layer_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->remove_layer(id);
}

void model::select_layer(const layer_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->select_layer(id);
}

void model::select_tool(const tool_id id)
{
  m_tools.select(id);
}

void model::select_tileset(const tileset_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->select_tileset(id);
}

void model::set_tileset_selection(const tileset_selection& selection)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->set_tileset_selection(selection);
}

void model::increase_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->increase_tile_size();
}

void model::decrease_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->decrease_tile_size();
}

void model::reset_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->reset_tile_size();
}

void model::create_tileset(const QImage& image,
                           const QFileInfo& path,
                           const QString& name,
                           const tile_width tileWidth,
                           const tile_height tileHeight)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_tileset(image, path, name, tileWidth, tileHeight);
}

void model::remove_tileset(const tileset_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->ui_remove_tileset(id);
}

void model::set_tileset_name(const tileset_id id, const QString& name)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->set_tileset_name(id, name);
}

void model::select_map(const map_id id)
{
  Q_ASSERT(m_documents.contains(id));

  if (m_currentMap != id) {
    m_currentMap = id;
    emit_undo_redo_update();
    emit clean_changed(current_document()->is_clean());
  }

  auto* document = current_document();
  Q_ASSERT(document);

  emit switched_map(id, document);
}

void model::close_map(const map_id id)
{
  Q_ASSERT(m_documents.contains(id));

  m_documents.at(id)->disconnect();
  m_documents.erase(id);

  if (m_currentMap && (m_currentMap->get() == id.get())) {
    m_currentMap = std::nullopt;
    emit_undo_redo_update();
  }

  if (!m_documents.empty()) {
    m_currentMap = m_documents.begin()->first;
    emit_undo_redo_update();
  }
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

void model::emit_undo_redo_update()
{
  if (const auto* document = current_document()) {
    emit undo_state_updated(document->can_undo());
    emit redo_state_updated(document->can_redo());
    emit undo_text_updated(document->get_undo_text());
    emit redo_text_updated(document->get_redo_text());
  } else {
    emit undo_state_updated(false);
    emit redo_state_updated(false);
    emit undo_text_updated(TACTILE_QSTRING(u""));
    emit redo_text_updated(TACTILE_QSTRING(u""));
  }
}

}  // namespace tactile::core
