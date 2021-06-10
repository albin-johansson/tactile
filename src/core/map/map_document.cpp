#include "map_document.hpp"

#include <utility>  // move

#include "add_col.hpp"
#include "add_layer.hpp"
#include "add_property.hpp"
#include "add_row.hpp"
#include "add_tileset.hpp"
#include "bucket_fill.hpp"
#include "change_opacity.hpp"
#include "change_property_type.hpp"
#include "document_delegate.hpp"
#include "duplicate_layer.hpp"
#include "erase_sequence.hpp"
#include "move_layer_back.hpp"
#include "move_layer_forward.hpp"
#include "object_layer.hpp"
#include "remove_col.hpp"
#include "remove_layer.hpp"
#include "remove_property.hpp"
#include "remove_row.hpp"
#include "remove_tileset.hpp"
#include "rename_property.hpp"
#include "resize_map.hpp"
#include "select_layer.hpp"
#include "set_layer_name.hpp"
#include "set_layer_visibility.hpp"
#include "set_tileset_name.hpp"
#include "stamp_sequence.hpp"
#include "tile_layer.hpp"
#include "update_property.hpp"

namespace tactile::core {

map_document::map_document(QObject* parent)
    : document{parent}
    , m_map{std::make_unique<map>()}
    , m_tilesets{std::make_unique<tileset_manager>()}
    , m_delegate{std::make_unique<document_delegate>()}
{
  setup();
}

map_document::map_document(const row_t nRows,
                           const col_t nCols,
                           QObject* parent)
    : document{parent}
    , m_map{std::make_unique<map>(nRows, nCols)}
    , m_tilesets{std::make_unique<tileset_manager>()}
    , m_delegate{std::make_unique<document_delegate>()}
{
  setup();
}

void map_document::setup()
{
  auto* commands = m_delegate->history();

  // clang-format off
  connect(commands, &command_stack::cleanChanged, this, &map_document::clean_changed);
  connect(commands, &command_stack::canUndoChanged, this, &map_document::undo_state_updated);
  connect(commands, &command_stack::canRedoChanged, this, &map_document::redo_state_updated);
  connect(commands, &command_stack::undoTextChanged, this, &map_document::undo_text_updated);
  connect(commands, &command_stack::redoTextChanged, this, &map_document::redo_text_updated);

  connect(m_delegate.get(), &document_delegate::added_property,
          this, &map_document::added_property);

  connect(m_delegate.get(), &document_delegate::about_to_remove_property,
          this, &map_document::about_to_remove_property);

  connect(m_delegate.get(), &document_delegate::updated_property,
          this, &map_document::updated_property);

  connect(m_delegate.get(), &document_delegate::renamed_property,
          this, &map_document::renamed_property);

  connect(m_delegate.get(), &document_delegate::changed_property_type,
          this, &map_document::changed_property_type);
  // clang-format on
}

void map_document::undo()
{
  m_delegate->undo();

  /* Emit clean_changed once more, because we need to take into account that the
     document might not feature an associated file path yet (that is what
     is_clean does) */
  emit clean_changed(is_clean());
}

void map_document::redo()
{
  m_delegate->redo();
}

void map_document::mark_as_clean()
{
  m_delegate->mark_as_clean();
}

void map_document::reset_history()
{
  m_delegate->reset_history();
}

void map_document::set_path(QFileInfo path)
{
  m_delegate->set_path(path);
}

auto map_document::can_undo() const -> bool
{
  return m_delegate->can_undo();
}

auto map_document::can_redo() const -> bool
{
  return m_delegate->can_redo();
}

auto map_document::is_clean() const -> bool
{
  return m_delegate->is_clean();
}

auto map_document::has_path() const -> bool
{
  return m_delegate->has_path();
}

auto map_document::get_undo_text() const -> QString
{
  return m_delegate->get_undo_text();
}

auto map_document::get_redo_text() const -> QString
{
  return m_delegate->get_redo_text();
}

auto map_document::path() const -> const QFileInfo&
{
  return m_delegate->path();
}

auto map_document::absolute_path() const -> QString
{
  return m_delegate->absolute_path();
}

void map_document::add_property(const QString& name,
                                const core::property_type type)
{
  const QSignalBlocker blocker{m_delegate.get()};
  m_delegate->execute<cmd::add_property>(m_delegate.get(), name, type);
}

void map_document::add_property(const QString& name,
                                const core::property& property)
{
  const QSignalBlocker blocker{m_delegate.get()};
  m_delegate->execute<cmd::add_property>(m_delegate.get(), name, property);
}

void map_document::remove_property(const QString& name)
{
  const QSignalBlocker blocker{m_delegate.get()};
  m_delegate->execute<cmd::remove_property>(m_delegate.get(), name);
}

void map_document::rename_property(const QString& oldName,
                                   const QString& newName)
{
  const QSignalBlocker blocker{m_delegate.get()};
  m_delegate->execute<cmd::rename_property>(m_delegate.get(), oldName, newName);
}

void map_document::set_property(const QString& name,
                                const core::property& property)
{
  const QSignalBlocker blocker{m_delegate.get()};
  m_delegate->execute<cmd::update_property>(m_delegate.get(), name, property);
}

void map_document::change_property_type(const QString& name,
                                        const core::property_type type)
{
  const QSignalBlocker blocker{m_delegate.get()};
  m_delegate->execute<cmd::change_property_type>(m_delegate.get(), name, type);
}

auto map_document::get_property(const QString& name) const
    -> const core::property&
{
  return m_delegate->get_property(name);
}

auto map_document::get_property(const QString& name) -> core::property&
{
  return m_delegate->get_property(name);
}

auto map_document::has_property(const QString& name) const -> bool
{
  return m_delegate->has_property(name);
}

auto map_document::property_count() const noexcept -> int
{
  return m_delegate->property_count();
}

auto map_document::properties() const -> const property_map&
{
  return m_delegate->properties();
}

void map_document::flood(const position& position, const tile_id replacement)
{
  m_delegate->execute<cmd::bucket_fill>(this, position, replacement);
}

void map_document::add_stamp_sequence(vector_map<position, tile_id>&& oldState,
                                      vector_map<position, tile_id>&& sequence)
{
  m_delegate->execute<cmd::stamp_sequence>(this,
                                           std::move(oldState),
                                           std::move(sequence));
}

void map_document::add_erase_sequence(vector_map<position, tile_id>&& oldState)
{
  m_delegate->execute<cmd::erase_sequence>(this, std::move(oldState));
}

void map_document::add_row()
{
  m_delegate->execute<cmd::AddRow>(this);
}

void map_document::add_column()
{
  m_delegate->execute<cmd::AddCol>(this);
}

void map_document::remove_row()
{
  m_delegate->execute<cmd::remove_row>(this);
}

void map_document::remove_column()
{
  m_delegate->execute<cmd::remove_col>(this);
}

void map_document::resize(const row_t nRows, const col_t nCols)
{
  Q_ASSERT(nRows > 0_row);
  Q_ASSERT(nCols > 0_col);
  m_delegate->execute<cmd::resize_map>(this, nRows, nCols);
}

void map_document::add_tileset(const QImage& image,
                               const QFileInfo& path,
                               const QString& name,
                               const tile_width tileWidth,
                               const tile_height tileHeight)
{
  if (!image.isNull())
  {
    const auto id = m_tilesets->next_tileset_id();
    const auto gid = m_tilesets->next_global_tile_id();

    auto ts = std::make_shared<tileset>(gid, image, tileWidth, tileHeight);
    ts->set_name(name);
    ts->set_path(path);

    // This will cause an `added_tileset` signal to be emitted
    m_delegate->execute<cmd::add_tileset>(this, std::move(ts), id);
    m_tilesets->increment_next_tileset_id();
  }
}

void map_document::remove_tileset(tileset_id id)
{
  m_delegate->execute<cmd::remove_tileset>(this,
                                           m_tilesets->get_tileset_pointer(id),
                                           id);
}

void map_document::select_tileset(const tileset_id id)
{
  m_tilesets->select(id);
}

void map_document::set_tileset_selection(const tileset_selection& selection)
{
  m_tilesets->set_selection(selection);
}

void map_document::select_layer(const layer_id id)
{
  m_delegate->execute<cmd::SelectLayer>(this, id);
}

void map_document::add_layer(const layer_id id, const shared<layer>& layer)
{
  Q_ASSERT(layer);
  m_map->add_layer(id, layer);
}

auto map_document::add_tile_layer() -> layer_id
{
  const auto id = m_map->next_layer_id();  // must be before make_tile_layer
  auto layer = m_map->make_tile_layer();

  layer->set_name(layer->name() + TACTILE_QSTRING(u" ") +
                  QString::number(m_tileLayerSuffix));
  ++m_tileLayerSuffix;

  m_delegate->execute<cmd::AddLayer>(this, std::move(layer), id);
  return id;
}

auto map_document::add_object_layer() -> layer_id
{
  const auto id = m_map->next_layer_id();  // must be before make_object_layer
  auto layer = m_map->make_object_layer();

  layer->set_name(layer->name() + TACTILE_QSTRING(u" ") +
                  QString::number(m_objectLayerSuffix));
  ++m_objectLayerSuffix;

  m_delegate->execute<cmd::AddLayer>(this, std::move(layer), id);
  return id;
}

void map_document::remove_layer(const layer_id id)
{
  m_delegate->execute<cmd::RemoveLayer>(this, id);
}

auto map_document::take_layer(const layer_id id) -> shared<layer>
{
  return m_map->take_layer(id);
}

void map_document::duplicate_layer(const layer_id id)
{
  m_delegate->execute<cmd::DuplicateLayer>(this, id);
}

void map_document::increase_tile_size()
{
  m_map->increase_tile_size();
  emit redraw();
}

void map_document::decrease_tile_size()
{
  m_map->decrease_tile_size();
  emit redraw();
}

void map_document::reset_tile_size()
{
  m_map->reset_tile_size();
  emit redraw();
}

void map_document::set_layer_visibility(const layer_id id, const bool visible)
{
  m_delegate->execute<cmd::SetLayerVisibility>(this, id, visible);
}

void map_document::set_layer_opacity(const layer_id id, const double opacity)
{
  m_delegate->execute<cmd::ChangeOpacity>(this, id, opacity);
}

void map_document::set_layer_name(const layer_id id, const QString& name)
{
  m_delegate->execute<cmd::SetLayerName>(this, id, name);
}

void map_document::move_layer_back(const layer_id id)
{
  m_delegate->execute<cmd::MoveLayerBack>(this, id);
}

void map_document::move_layer_forward(const layer_id id)
{
  m_delegate->execute<cmd::MoveLayerForward>(this, id);
}

void map_document::set_tileset_name(const tileset_id id, const QString& name)
{
  m_delegate->execute<cmd::set_tileset_name>(this, id, name);
}

void map_document::set_next_layer_id(const layer_id id) noexcept
{
  m_map->set_next_layer_id(id);
}

void map_document::set_next_object_id(const object_id id) noexcept
{
  m_map->set_next_object_id(id);
}

auto map_document::in_bounds(const position& pos) const -> bool
{
  return m_map->in_bounds(pos);
}

auto map_document::get_layer(const layer_id id) -> layer*
{
  return m_map->get_layer(id).get();
}

auto map_document::get_layer(const layer_id id) const -> const layer*
{
  return m_map->get_layer(id).get();
}

auto map_document::get_tile_layer(const layer_id id) -> tile_layer*
{
  return m_map->get_tile_layer(id);
}

auto map_document::get_tile_layer(const layer_id id) const -> const tile_layer*
{
  return m_map->get_tile_layer(id);
}

auto map_document::get_object_layer(const layer_id id) const
    -> const object_layer*
{
  return m_map->get_object_layer(id);
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

auto map_document::get_map() const noexcept -> const map*
{
  return m_map.get();
}

auto map_document::tilesets() const noexcept -> const tileset_manager*
{
  return m_tilesets.get();
}

auto map_document::tilesets() noexcept -> tileset_manager*
{
  return m_tilesets.get();
}

auto map_document::raw() -> map&
{
  return *m_map;
}

auto map_document::current_layer_id() const noexcept -> maybe<layer_id>
{
  return m_map->active_layer_id();
}

auto map_document::has_layer(const layer_id id) const -> bool
{
  return m_map->has_layer(id);
}

}  // namespace tactile::core
