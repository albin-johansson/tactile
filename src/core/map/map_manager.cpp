#include "map_manager.hpp"

namespace tactile::core {

map_manager::map_manager(QObject* parent) : QObject{parent}
{}

void map_manager::emit_undo_redo_update()
{
  const auto* document = current_document();

  emit undo_state_updated(document && document->can_undo());
  emit redo_state_updated(document && document->can_redo());
  emit undo_text_updated(document ? document->undo_text()
                                  : QStringLiteral(u""));
  emit redo_text_updated(document ? document->redo_text()
                                  : QStringLiteral(u""));
}

auto map_manager::add() -> map_id
{
  return add(new map_document{5_row, 5_col, this});
}

auto map_manager::add(map_document* document) -> map_id
{
  Q_ASSERT(document);

  const auto id = m_nextMapID;
  Q_ASSERT(!m_mapDocuments.contains(id));

  document->setParent(this);

  const auto bind = [document, this](auto&& signal, auto&& slot) {
    connect(document, signal, this, slot);
  };

  bind(&map_document::undo_state_updated, &map_manager::undo_state_updated);
  bind(&map_document::redo_state_updated, &map_manager::redo_state_updated);
  bind(&map_document::undo_text_updated, &map_manager::undo_text_updated);
  bind(&map_document::redo_text_updated, &map_manager::redo_text_updated);
  bind(&map_document::added_tileset, &map_manager::added_tileset);
  bind(&map_document::removed_tileset, &map_manager::removed_tileset);
  bind(&map_document::added_layer, &map_manager::added_layer);
  bind(&map_document::added_duplicated_layer,
       &map_manager::added_duplicated_layer);
  bind(&map_document::removed_layer, &map_manager::removed_layer);
  bind(&map_document::selected_layer, &map_manager::selected_layer);
  bind(&map_document::moved_layer_back, &map_manager::moved_layer_back);
  bind(&map_document::moved_layer_forward, &map_manager::moved_layer_forward);

  m_mapDocuments.emplace(id, document);
  m_currentMapID = id;

  ++m_nextMapID;

  emit_undo_redo_update();

  return id;
}

void map_manager::close(map_id id)
{
  Q_ASSERT(m_mapDocuments.contains(id));

  m_mapDocuments.at(id)->disconnect();
  m_mapDocuments.erase(id);

  if (m_currentMapID && (m_currentMapID->get() == id.get())) {
    m_currentMapID = std::nullopt;
    emit_undo_redo_update();
  }

  if (!m_mapDocuments.empty()) {
    const auto begin = m_mapDocuments.begin();
    m_currentMapID = begin->first;
    emit_undo_redo_update();
  }
}

void map_manager::select(map_id id)
{
  Q_ASSERT(m_mapDocuments.contains(id));

  if (m_currentMapID != id) {
    m_currentMapID = id;
    emit_undo_redo_update();
  }
}

void map_manager::add_tileset(const QImage& image,
                              const QFileInfo& path,
                              const QString& name,
                              tile_width tileWidth,
                              tile_height tileHeight)
{
  if (auto* document = current_document()) {
    document->add_tileset(image, path, name, tileWidth, tileHeight);
  }
}

void map_manager::ui_remove_tileset(tileset_id id)
{
  if (auto* document = current_document()) {
    document->ui_remove_tileset(id);
  }
}

void map_manager::select_tileset(tileset_id id)
{
  if (auto* document = current_document()) {
    document->select_tileset(id);
  }
}

void map_manager::set_layer_visibility(layer_id id, bool visible)
{
  if (auto* document = current_document()) {
    document->set_layer_visibility(id, visible);
  }
}

void map_manager::set_layer_opacity(layer_id id, double opacity)
{
  if (auto* document = current_document()) {
    document->set_layer_opacity(id, opacity);
  }
}

void map_manager::set_layer_name(layer_id id, const QString& name)
{
  if (auto* document = current_document()) {
    document->set_layer_name(id, name);
  }
}

void map_manager::move_layer_back(layer_id id)
{
  if (auto* document = current_document()) {
    document->move_layer_back(id);
  }
}

void map_manager::move_layer_forward(layer_id id)
{
  if (auto* document = current_document()) {
    document->move_layer_forward(id);
  }
}

void map_manager::set_tileset_selection(const tileset::selection& selection)
{
  if (auto* document = current_document()) {
    document->set_selection(selection);
  }
}

auto map_manager::has_active_map() const noexcept -> bool
{
  return m_currentMapID.has_value();
}

auto map_manager::at(map_id id) -> map_document*
{
  if (const auto it = m_mapDocuments.find(id); it != m_mapDocuments.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

auto map_manager::at(map_id id) const -> const map_document*
{
  if (const auto it = m_mapDocuments.find(id); it != m_mapDocuments.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

auto map_manager::current_map_id() const -> std::optional<map_id>
{
  return m_currentMapID;
}

auto map_manager::current_document() -> map_document*
{
  return m_currentMapID ? m_mapDocuments.at(m_currentMapID.value()) : nullptr;
}

auto map_manager::current_document() const -> const map_document*
{
  return m_currentMapID ? m_mapDocuments.at(m_currentMapID.value()) : nullptr;
}

auto map_manager::current_tileset() const -> const tileset*
{
  const auto* document = current_document();
  return document ? document->current_tileset() : nullptr;
}

}  // namespace tactile::core
