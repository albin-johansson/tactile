#include "map_document.hpp"

#include <QDebug>
#include <utility>  // move

#include "add_col.hpp"
#include "add_layer.hpp"
#include "add_row.hpp"
#include "add_tileset.hpp"
#include "bucket_fill.hpp"
#include "erase_sequence.hpp"
#include "property_model.hpp"
#include "remove_col.hpp"
#include "remove_layer.hpp"
#include "remove_row.hpp"
#include "remove_tileset.hpp"
#include "resize_map.hpp"
#include "stamp_sequence.hpp"

namespace tactile::core {

map_document::map_document(QObject* parent)
    : QObject{parent}
    , m_map{std::make_unique<map>()}
    , m_tilesets{std::make_unique<tileset_manager>()}
{
  setup();
}

map_document::map_document(const row_t nRows,
                           const col_t nCols,
                           QObject* parent)
    : QObject{parent}
    , m_map{std::make_unique<map>(nRows, nCols)}
    , m_tilesets{std::make_unique<tileset_manager>()}
{
  setup();
}

void map_document::setup()
{
  auto* commands = m_delegate.get_commands();
  // clang-format off
  connect(commands, &command_stack::cleanChanged,    this, &map_document::clean_changed);
  connect(commands, &command_stack::canUndoChanged,  this, &map_document::undo_state_updated);
  connect(commands, &command_stack::canRedoChanged,  this, &map_document::redo_state_updated);
  connect(commands, &command_stack::undoTextChanged, this, &map_document::undo_text_updated);
  connect(commands, &command_stack::redoTextChanged, this, &map_document::redo_text_updated);
  // clang-format on
}

void map_document::undo()
{
  m_delegate.undo();

  /* Emit clean_changed once more, because we need to take into account that the
     document might not feature an associated file path yet (that is what
     is_clean does) */
  emit clean_changed(is_clean());
}

void map_document::redo()
{
  m_delegate.redo();
}

void map_document::mark_as_clean()
{
  m_delegate.mark_as_clean();
}

void map_document::set_path(QFileInfo path)
{
  m_delegate.set_path(std::move(path));
}

auto map_document::can_undo() const -> bool
{
  return m_delegate.can_undo();
}

auto map_document::can_redo() const -> bool
{
  return m_delegate.can_redo();
}

auto map_document::is_clean() const -> bool
{
  return m_delegate.is_clean();
}

auto map_document::get_undo_text() const -> QString
{
  return m_delegate.get_undo_text();
}

auto map_document::get_redo_text() const -> QString
{
  return m_delegate.get_redo_text();
}

auto map_document::path() const -> const QFileInfo&
{
  return m_delegate.path();
}

void map_document::add_property(const QString& name, const property::type type)
{
  m_delegate.add_property(name, type);
}

void map_document::remove_property(const QString& name)
{
  m_delegate.remove_property(name);
}

void map_document::rename_property(const QString& oldName,
                                   const QString& newName)
{
  m_delegate.rename_property(oldName, newName);
}

void map_document::set_property(const QString& name,
                                const core::property& property)
{
  m_delegate.set_property(name, property);
}

auto map_document::get_property(const QString& name) const
    -> const core::property&
{
  return m_delegate.get_property(name);
}

auto map_document::get_property(const QString& name) -> core::property&
{
  return m_delegate.get_property(name);
}

auto map_document::property_count() const noexcept -> int
{
  return m_delegate.property_count();
}

auto map_document::properties() const -> const property_map&
{
  return m_delegate.properties();
}

void map_document::flood(const position& position, const tile_id replacement)
{
  m_delegate.get_commands()->push<cmd::bucket_fill>(m_map.get(),
                                                    position,
                                                    replacement);
}

void map_document::add_stamp_sequence(vector_map<position, tile_id>&& oldState,
                                      vector_map<position, tile_id>&& sequence)
{
  m_delegate.get_commands()->push<cmd::stamp_sequence>(m_map.get(),
                                                       std::move(oldState),
                                                       std::move(sequence));
}

void map_document::add_erase_sequence(vector_map<position, tile_id>&& oldState)
{
  m_delegate.get_commands()->push<cmd::erase_sequence>(m_map.get(),
                                                       std::move(oldState));
}

void map_document::add_row()
{
  m_delegate.get_commands()->push<cmd::add_row>(m_map.get());
}

void map_document::add_column()
{
  m_delegate.get_commands()->push<cmd::add_col>(m_map.get());
}

void map_document::remove_row()
{
  m_delegate.get_commands()->push<cmd::remove_row>(m_map.get());
}

void map_document::remove_column()
{
  m_delegate.get_commands()->push<cmd::remove_col>(m_map.get());
}

void map_document::resize(const row_t nRows, const col_t nCols)
{
  Q_ASSERT(nRows > 0_row);
  Q_ASSERT(nCols > 0_col);
  m_delegate.get_commands()->push<cmd::resize_map>(m_map.get(), nRows, nCols);
}

void map_document::add_tileset(const QImage& image,
                               const QFileInfo& path,
                               const QString& name,
                               const tile_width tileWidth,
                               const tile_height tileHeight)
{
  if (!image.isNull()) {
    const auto id = m_tilesets->next_tileset_id();
    const auto gid = m_tilesets->next_global_tile_id();

    auto ts = std::make_shared<tileset>(gid, image, tileWidth, tileHeight);
    ts->set_name(name);
    ts->set_path(path);

    // This will cause an `added_tileset` signal to be emitted
    m_delegate.get_commands()->push<cmd::add_tileset>(this, std::move(ts), id);
    m_tilesets->increment_next_tileset_id();
  }
}

void map_document::add_tileset(const tileset_id id, shared_tileset tileset)
{
  m_tilesets->add(id, std::move(tileset));
  emit added_tileset(id);
}

void map_document::add_tileset(shared_tileset tileset)
{
  const auto id = m_tilesets->add(std::move(tileset));
  emit added_tileset(id);
}

void map_document::remove_tileset(const tileset_id id)
{
  Q_ASSERT(m_tilesets->contains(id));

  const auto [first, last] = m_tilesets->range_of(id);

  for (auto i = first; i < last; ++i) {
    m_map->remove_occurrences(i);
  }

  m_tilesets->remove(id);

  emit removed_tileset(id);
}

void map_document::ui_remove_tileset(const tileset_id id)
{
  m_delegate.get_commands()->push<cmd::remove_tileset>(
      this,
      m_tilesets->get_tileset_pointer(id),
      id);
}

void map_document::select_tileset(const tileset_id id)
{
  m_tilesets->select(id);
}

void map_document::set_tileset_selection(const tileset::selection& selection)
{
  m_tilesets->set_selection(selection);
}

void map_document::select_layer(const layer_id id)
{
  m_map->select_layer(id);
  emit selected_layer(id, *m_map->get_layer(id));
}

void map_document::add_layer(const layer_id id, const shared_layer& layer)
{
  Q_ASSERT(layer);
  m_map->add_layer(id, layer);
  emit added_layer(id, *layer);
}

void map_document::add_layer()
{
  const auto id = m_map->next_layer_id();  // must be before make_tile_layer
  m_delegate.get_commands()->push<cmd::add_layer>(this,
                                                  m_map->make_tile_layer(),
                                                  id);
}

void map_document::remove_layer(const layer_id id)
{
  m_delegate.get_commands()->push<cmd::remove_layer>(this, id);
}

auto map_document::take_layer(const layer_id id) -> shared_layer
{
  return m_map->take_layer(id);
}

void map_document::duplicate_layer(const layer_id id)
{
  const auto& [newId, layer] = m_map->duplicate_layer(id);
  emit added_duplicated_layer(newId, *layer);
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

void map_document::set_layer_visibility(const layer_id id, const bool visible)
{
  m_map->set_visibility(id, visible);
}

void map_document::set_layer_opacity(const layer_id id, const double opacity)
{
  m_map->set_opacity(id, opacity);
}

void map_document::set_layer_name(const layer_id id, const QString& name)
{
  m_map->set_name(id, name);
}

void map_document::move_layer_back(const layer_id id)
{
  m_map->move_layer_back(id);
  emit moved_layer_back(id);
}

void map_document::move_layer_forward(const layer_id id)
{
  m_map->move_layer_forward(id);
  emit moved_layer_forward(id);
}

void map_document::set_tileset_name(const tileset_id id, const QString& name)
{
  m_tilesets->rename(id, name);
}

void map_document::set_next_layer_id(const layer_id id) noexcept
{
  m_map->set_next_layer_id(id);
}

auto map_document::index_of_layer(const layer_id id) const -> maybe<int>
{
  return m_map->index_of(id);
}

auto map_document::in_bounds(const position& pos) const -> bool
{
  return m_map->in_bounds(pos);
}

auto map_document::get_layer(const layer_id id) const -> const layer&
{
  return *m_map->get_layer(id);
}

auto map_document::get_tile_layer(const layer_id id) -> tile_layer*
{
  return m_map->get_tile_layer(id);
}

auto map_document::get_tile_layer(const layer_id id) const -> const tile_layer*
{
  return m_map->get_tile_layer(id);
}

auto map_document::layer_count() const noexcept -> int
{
  return m_map->layer_count();
}

auto map_document::row_count() const -> row_t
{
  return m_map->row_count();
}

auto map_document::col_count() const -> col_t
{
  return m_map->col_count();
}

auto map_document::width() const -> int
{
  return m_map->width();
}

auto map_document::height() const -> int
{
  return m_map->height();
}

auto map_document::current_tile_size() const noexcept -> int
{
  return m_map->current_tile_size();
}

auto map_document::current_tileset() const -> const tileset*
{
  return m_tilesets->current_tileset();
}

auto map_document::tilesets() const noexcept -> const tileset_manager*
{
  return m_tilesets.get();
}

auto map_document::current_layer_id() const noexcept -> maybe<layer_id>
{
  return m_map->active_layer_id();
}

}  // namespace tactile::core
