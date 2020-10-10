#include "map_manager.hpp"

namespace tactile::core {

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
  const auto id = m_nextMapID;
  Q_ASSERT(!m_mapDocuments.contains(id));

  auto* document = new map_document{this};

  const auto bind = [document, this](auto&& signal, auto&& slot) {
    connect(document, signal, this, slot);
  };

  bind(&map_document::undo_state_updated, &map_manager::undo_state_updated);
  bind(&map_document::redo_state_updated, &map_manager::redo_state_updated);
  bind(&map_document::undo_text_updated, &map_manager::undo_text_updated);
  bind(&map_document::redo_text_updated, &map_manager::redo_text_updated);
  bind(&map_document::added_tileset, &map_manager::added_tileset);
  bind(&map_document::removed_tileset, &map_manager::removed_tileset);

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

void map_manager::ui_added_tileset(const QImage& image,
                                     const QString& path,
                                     const QString& name,
                                     tile_width tileWidth,
                                     tile_height tileHeight)
{
  if (auto* document = current_document()) {
    document->add_tileset(image, path, name, tileWidth, tileHeight);
  }
}

void map_manager::ui_removed_tileset(tileset_id id)
{
  if (auto* document = current_document()) {
    document->remove_tileset(id);
  }
}

void map_manager::select_tileset(tileset_id id)
{
  if (auto* document = current_document()) {
    document->select_tileset(id);
  }
}

void map_manager::update_tileset_selection(position topLeft,
                                           position bottomRight)
{
  if (auto* document = current_document()) {
    document->set_selection(topLeft, bottomRight);
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

auto map_manager::current_map() const -> std::optional<map_id>
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
