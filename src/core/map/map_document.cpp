#include "map_document.hpp"

#include <qdebug.h>

#include "add_col.hpp"
#include "add_row.hpp"
#include "add_tileset.hpp"
#include "bucket_fill.hpp"
#include "erase_sequence.hpp"
#include "remove_col.hpp"
#include "remove_row.hpp"
#include "remove_tileset.hpp"
#include "resize_map.hpp"
#include "stamp_sequence.hpp"

namespace tactile::core {

map_document::map_document(QObject* parent)
    : QObject{parent},
      m_map{std::make_unique<map>()},
      m_tilesets{std::make_unique<tileset_manager>()},
      m_commands{new command_stack{this}}
{
  setup();
}

map_document::map_document(row_t nRows, col_t nCols, QObject* parent)
    : QObject{parent},
      m_map{std::make_unique<map>(nRows, nCols)},
      m_tilesets{std::make_unique<tileset_manager>()},
      m_commands{new command_stack{this}}
{
  setup();
}

void map_document::setup()
{
  m_commands->setUndoLimit(100);

  // clang-format off
  connect(m_commands, &command_stack::canUndoChanged, this, &map_document::undo_state_updated);
  connect(m_commands, &command_stack::canRedoChanged, this, &map_document::redo_state_updated);
  connect(m_commands, &command_stack::undoTextChanged, this, &map_document::undo_text_updated);
  connect(m_commands, &command_stack::redoTextChanged, this, &map_document::redo_text_updated);
  // clang-format on
}

void map_document::undo()
{
  m_commands->undo();
}

void map_document::redo()
{
  m_commands->redo();
}

void map_document::flood(const position& position, tile_id replacement)
{
  m_commands->push<cmd::bucket_fill>(m_map.get(), position, replacement);
}

void map_document::add_stamp_sequence(vector_map<position, tile_id>&& oldState,
                                      vector_map<position, tile_id>&& sequence)
{
  m_commands->push<cmd::stamp_sequence>(m_map.get(),
                                        std::move(oldState),
                                        std::move(sequence));
}

void map_document::add_erase_sequence(vector_map<position, tile_id>&& oldState)
{
  m_commands->push<cmd::erase_sequence>(m_map.get(), std::move(oldState));
}

void map_document::add_row()
{
  m_commands->push<cmd::add_row>(m_map.get());
}

void map_document::add_column()
{
  m_commands->push<cmd::add_col>(m_map.get());
}

void map_document::remove_row()
{
  m_commands->push<cmd::remove_row>(m_map.get());
}

void map_document::remove_column()
{
  m_commands->push<cmd::remove_col>(m_map.get());
}

void map_document::resize(row_t nRows, col_t nCols)
{
  Q_ASSERT(nRows > 0_row);
  Q_ASSERT(nCols > 0_col);
  m_commands->push<cmd::resize_map>(m_map.get(), nRows, nCols);
}

void map_document::add_tileset(const QImage& image,
                               const QFileInfo& path,
                               const QString& name,
                               tile_width tileWidth,
                               tile_height tileHeight)
{
  if (!image.isNull()) {
    const auto id = m_tilesets->next_tileset_id();
    const auto gid = m_tilesets->next_global_tile_id();

    auto ts = std::make_shared<tileset>(gid, image, tileWidth, tileHeight);
    ts->set_name(name);
    ts->set_path(path);

    // This will cause an `added_tileset` signal to be emitted
    m_commands->push<cmd::add_tileset>(this, std::move(ts), id);
    m_tilesets->increment_next_tileset_id();
  }
}

void map_document::add_tileset(tileset_id id, std::shared_ptr<tileset> tileset)
{
  m_tilesets->add(id, std::move(tileset));
  emit added_tileset(id);
}

void map_document::add_tileset(std::shared_ptr<tileset> tileset)
{
  const auto id = m_tilesets->add(std::move(tileset));
  emit added_tileset(id);
}

void map_document::remove_tileset(tileset_id id, bool notify)
{
  Q_ASSERT(m_tilesets->contains(id));

  const auto [first, last] = m_tilesets->range_of(id);

  for (auto i = first; i < last; ++i) {
    m_map->remove_occurrences(i);
  }

  m_tilesets->remove(id);

  if (notify) {
    emit removed_tileset(id);
  }
}

void map_document::ui_removed_tileset(tileset_id id)
{
  m_commands->push<cmd::remove_tileset>(this, m_tilesets->get_ptr(id), id);
}

void map_document::select_tileset(tileset_id id)
{
  m_tilesets->select(id);
}

void map_document::set_selection(const tileset::selection& selection)
{
  m_tilesets->set_selection(selection);
}

void map_document::select_layer(layer_id id)
{
  m_map->select_layer(id);
  emit selected_layer(id, m_map->get_layer(id));
}

void map_document::add_layer(layer_id id, layer&& layer)
{
  m_map->add_layer(id, std::move(layer));
  emit added_layer(id, m_map->get_layer(id));
}

void map_document::add_layer()
{
  const auto id = m_map->add_layer();
  emit added_layer(id, m_map->get_layer(id));
}

void map_document::remove_layer(layer_id id)
{
  m_map->remove_layer(id);
  emit removed_layer(id);
}

void map_document::duplicate_layer(layer_id id)
{
  const auto& [newId, layer] = m_map->duplicate_layer(id);
  emit added_duplicated_layer(newId, layer);
}

void map_document::increase_tile_size()
{
  m_map->increase_tile_size();
}

void map_document::decrease_tile_size()
{
  m_map->decrease_tile_size();
}

void map_document::reset_tile_size()
{
  m_map->reset_tile_size();
}

void map_document::set_layer_visibility(layer_id id, bool visible)
{
  m_map->set_visibility(id, visible);
}

void map_document::set_layer_opacity(layer_id id, double opacity)
{
  m_map->set_opacity(id, opacity);
}

void map_document::set_layer_name(layer_id id, const QString& name)
{
  m_map->set_name(id, name);
}

void map_document::move_layer_back(layer_id id)
{
  m_map->move_layer_back(id);
  emit moved_layer_back(id);
}

void map_document::move_layer_forward(layer_id id)
{
  m_map->move_layer_forward(id);
  emit moved_layer_forward(id);
}

auto map_document::can_undo() const -> bool
{
  return m_commands->canUndo();
}

auto map_document::can_redo() const -> bool
{
  return m_commands->canRedo();
}

auto map_document::undo_text() const -> QString
{
  return m_commands->undoText();
}

auto map_document::redo_text() const -> QString
{
  return m_commands->redoText();
}

}  // namespace tactile::core
