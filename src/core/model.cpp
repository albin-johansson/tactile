#include "model.hpp"

#include "layer_model.hpp"
#include "map_document.hpp"
#include "property_model.hpp"

namespace tactile::core {

model::model() : m_tools{this}
{}

auto model::add_map() -> map_id
{
  return add_map(new MapDocument{5_row, 5_col, this});
}

auto model::add_map(MapDocument* document) -> map_id
{
  Q_ASSERT(document);
  Q_ASSERT(!m_documents.contains(m_nextId));

  document->setParent(this);

  const auto bind = [document, this](auto&& signal, auto&& slot) {
    QObject::connect(document, signal, this, slot);
  };

  // clang-format off
  bind(&MapDocument::S_Redraw,                   &model::redraw);
  bind(&MapDocument::S_UndoStateUpdated,       &model::undo_state_updated);
  bind(&MapDocument::S_RedoStateUpdated,       &model::redo_state_updated);
  bind(&MapDocument::S_UndoTextUpdated,        &model::undo_text_updated);
  bind(&MapDocument::S_RedoTextUpdated,        &model::redo_text_updated);
  bind(&MapDocument::S_CleanChanged,            &model::clean_changed);
  bind(&MapDocument::S_RemovedTileset,          &model::removed_tileset);
  bind(&MapDocument::S_RenamedTileset,          &model::renamed_tileset);
  bind(&MapDocument::S_AddedLayer,              &model::added_layer);
  bind(&MapDocument::S_AddedDuplicatedLayer,   &model::added_duplicated_layer);
  bind(&MapDocument::S_RemovedLayer,            &model::removed_layer);
  bind(&MapDocument::S_SelectedLayer,           &model::selected_layer);
  bind(&MapDocument::S_MovedLayerBack,         &model::moved_layer_back);
  bind(&MapDocument::S_MovedLayerForward,      &model::moved_layer_forward);
  bind(&MapDocument::S_AddedProperty,           &model::added_property);
  bind(&MapDocument::S_AboutToRemoveProperty, &model::about_to_remove_property);
  bind(&MapDocument::S_UpdatedProperty,         &model::updated_property);
  bind(&MapDocument::S_RenamedProperty,         &model::renamed_property);
  bind(&MapDocument::S_ChangedPropertyType,    &model::changed_property_type);
  // clang-format on

  bind(&MapDocument::S_AddedTileset, [this](const tileset_id id) {
    const auto& tileset = current_document()->GetTilesets()->At(id);
    emit added_tileset(current_map_id().value(), id, tileset);
  });

  bind(&MapDocument::S_ShowProperties,
       [this] { emit show_map_properties(current_document()); });

  bind(&MapDocument::S_ShowLayerProperties, [this](const layer_id id) {
    emit show_layer_properties(current_document()->GetLayer(id));
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

auto model::get_document(const map_id id) -> MapDocument*
{
  return m_documents.at(id);
}

auto model::current_map_id() const -> maybe<map_id>
{
  return m_currentMap;
}

auto model::current_document() -> MapDocument*
{
  return m_currentMap ? m_documents.at(m_currentMap.value()) : nullptr;
}

auto model::current_document() const -> const MapDocument*
{
  return m_currentMap ? m_documents.at(m_currentMap.value()) : nullptr;
}

void model::undo()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->Undo();
}

void model::redo()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->Redo();
}

void model::resize_map(const row_t nRows, const col_t nCols)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->Resize(nRows, nCols);
}

void model::add_row()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->AddRow();
}

void model::add_col()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->AddColumn();
}

void model::remove_row()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->RemoveRow();
}

void model::remove_col()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->RemoveColumn();
}

void model::add_layer()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->AddTileLayer();
}

void model::remove_layer(const layer_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->RemoveLayer(id);
}

void model::select_layer(const layer_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->SelectLayer(id);
}

void model::select_tool(const tool_id id)
{
  m_tools.select(id);
}

void model::select_tileset(const tileset_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->SelectTileset(id);
}

void model::set_tileset_selection(const TilesetSelection& selection)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->SetTilesetSelection(selection);
}

void model::increase_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->IncreaseTileSize();
}

void model::decrease_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->DecreaseTileSize();
}

void model::reset_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->ResetTileSize();
}

void model::create_tileset(const QImage& image,
                           const QFileInfo& path,
                           const QString& name,
                           const tile_width tileWidth,
                           const tile_height tileHeight)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->AddTileset(image, path, name, tileWidth, tileHeight);
}

void model::remove_tileset(const tileset_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->RemoveTileset(id);
}

void model::set_tileset_name(const tileset_id id, const QString& name)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->SetTilesetName(id, name);
}

void model::select_map(const map_id id)
{
  Q_ASSERT(m_documents.contains(id));

  if (m_currentMap != id)
  {
    m_currentMap = id;
    emit_undo_redo_update();
    emit clean_changed(current_document()->IsClean());
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

  if (m_currentMap && (m_currentMap->get() == id.get()))
  {
    m_currentMap = std::nullopt;
    emit_undo_redo_update();
  }

  if (!m_documents.empty())
  {
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
  if (const auto* document = current_document())
  {
    emit undo_state_updated(document->CanUndo());
    emit redo_state_updated(document->CanRedo());
    emit undo_text_updated(document->GetUndoText());
    emit redo_text_updated(document->GetRedoText());
  }
  else
  {
    emit undo_state_updated(false);
    emit redo_state_updated(false);
    emit undo_text_updated(TACTILE_QSTRING(u""));
    emit redo_text_updated(TACTILE_QSTRING(u""));
  }
}

}  // namespace tactile::core
