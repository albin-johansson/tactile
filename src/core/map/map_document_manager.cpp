#include "map_document_manager.hpp"

#include "tactile_qstring.hpp"

namespace tactile::core {

map_document_manager::map_document_manager(QObject* parent) : QObject{parent}
{}

void map_document_manager::emit_undo_redo_update()
{
  if (const auto* document = current_document(); document) {
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

auto map_document_manager::add() -> map_id
{
  return add(new map_document{5_row, 5_col, this});
}

auto map_document_manager::add(map_document* document) -> map_id
{
  Q_ASSERT(document);
  Q_ASSERT(!m_documents.contains(m_nextId));

  document->setParent(this);

  const auto bind = [document, this](auto&& signal, auto&& slot) {
    connect(document, signal, this, slot);
  };

  // clang-format off
  bind(&map_document::redraw,                 &map_document_manager::redraw);
  bind(&map_document::undo_state_updated,     &map_document_manager::undo_state_updated);
  bind(&map_document::redo_state_updated,     &map_document_manager::redo_state_updated);
  bind(&map_document::undo_text_updated,      &map_document_manager::undo_text_updated);
  bind(&map_document::redo_text_updated,      &map_document_manager::redo_text_updated);
  bind(&map_document::clean_changed,          &map_document_manager::clean_changed);
  bind(&map_document::added_tileset,          &map_document_manager::added_tileset);
  bind(&map_document::removed_tileset,        &map_document_manager::removed_tileset);
  bind(&map_document::added_layer,            &map_document_manager::added_layer);
  bind(&map_document::added_duplicated_layer, &map_document_manager::added_duplicated_layer);
  bind(&map_document::removed_layer,          &map_document_manager::removed_layer);
  bind(&map_document::selected_layer,         &map_document_manager::selected_layer);
  bind(&map_document::moved_layer_back,       &map_document_manager::moved_layer_back);
  bind(&map_document::moved_layer_forward,    &map_document_manager::moved_layer_forward);
  bind(&map_document::added_property,         &map_document_manager::added_property);
  bind(&map_document::removed_property,       &map_document_manager::removed_property);
  // clang-format on

  const auto id = m_nextId;
  m_documents.emplace(id, document);
  m_current = id;

  ++m_nextId;

  emit_undo_redo_update();

  return id;
}

void map_document_manager::close(const map_id id)
{
  Q_ASSERT(m_documents.contains(id));

  m_documents.at(id)->disconnect();
  m_documents.erase(id);

  if (m_current && (m_current->get() == id.get())) {
    m_current = std::nullopt;
    emit_undo_redo_update();
  }

  if (!m_documents.empty()) {
    m_current = m_documents.begin()->first;
    emit_undo_redo_update();
  }
}

void map_document_manager::select(const map_id id)
{
  Q_ASSERT(m_documents.contains(id));

  if (m_current != id) {
    m_current = id;
    emit_undo_redo_update();
    emit clean_changed(current_document()->is_clean());
  }
}

auto map_document_manager::has_active_map() const noexcept -> bool
{
  return m_current.has_value();
}

auto map_document_manager::at(const map_id id) -> map_document*
{
  if (const auto it = m_documents.find(id); it != m_documents.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

auto map_document_manager::at(const map_id id) const -> const map_document*
{
  if (const auto it = m_documents.find(id); it != m_documents.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

auto map_document_manager::current_map_id() const -> maybe<map_id>
{
  return m_current;
}

auto map_document_manager::current_document() -> map_document*
{
  return m_current ? m_documents.at(m_current.value()) : nullptr;
}

auto map_document_manager::current_document() const -> const map_document*
{
  return m_current ? m_documents.at(m_current.value()) : nullptr;
}

auto map_document_manager::current_tileset() const -> const tileset*
{
  const auto* document = current_document();
  return document ? document->current_tileset() : nullptr;
}

}  // namespace tactile::core
