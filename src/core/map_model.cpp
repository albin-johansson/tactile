#include "map_model.hpp"

#include "add_col.hpp"
#include "add_row.hpp"
#include "remove_col.hpp"
#include "remove_row.hpp"
#include "resize_map.hpp"

namespace tactile::model {

map_model::map_model(QObject* parent)
    : QObject{parent},
      m_map{(std::make_unique<map>(5, 5))},
      m_commands{new command_stack{this}}
{
  connect(m_commands,
          &command_stack::canUndoChanged,
          this,
          &map_model::undo_state_updated);

  connect(m_commands,
          &command_stack::canRedoChanged,
          this,
          &map_model::redo_state_updated);

  connect(m_commands,
          &command_stack::undoTextChanged,
          this,
          &map_model::undo_text_updated);

  connect(m_commands,
          &command_stack::redoTextChanged,
          this,
          &map_model::redo_text_updated);
}

void map_model::undo()
{
  m_commands->undo();
}

void map_model::redo()
{
  m_commands->redo();
}

void map_model::add_row()
{
  m_commands->push<cmd::add_row>(m_map.get());
}

void map_model::add_column()
{
  m_commands->push<cmd::add_col>(m_map.get());
}

void map_model::remove_row()
{
  m_commands->push<cmd::remove_row>(m_map.get());
}

void map_model::remove_column()
{
  m_commands->push<cmd::remove_col>(m_map.get());
}

void map_model::resize(int nRows, int nCols)
{
  m_commands->push<cmd::resize_map>(m_map.get(), nRows, nCols);
}

auto map_model::can_undo() const -> bool
{
  return m_commands->canUndo();
}

auto map_model::can_redo() const -> bool
{
  return m_commands->canRedo();
}

auto map_model::undo_text() const -> QString
{
  return m_commands->undoText();
}

auto map_model::redo_text() const -> QString
{
  return m_commands->redoText();
}

auto map_model::rows() const noexcept -> int
{
  return m_map->rows();
}

auto map_model::columns() const noexcept -> int
{
  return m_map->cols();
}

auto map_model::width() const noexcept -> int
{
  return m_map->width();
}

auto map_model::height() const noexcept -> int
{
  return m_map->height();
}

void map_model::select_layer(layer_id id)
{
  Q_ASSERT(has_layer(id));
  m_map->select(id);
}

auto map_model::has_layer(layer_id id) const noexcept -> bool
{
  return m_map->has_layer(id);
}

void map_model::reset_tile_size()
{
  m_map->get_tile_size().reset();
}

void map_model::increase_tile_size()
{
  m_map->get_tile_size().increase();
}

void map_model::decrease_tile_size()
{
  m_map->get_tile_size().decrease();
}

auto map_model::current_tile_size() const noexcept -> int
{
  return m_map->get_tile_size().get();
}

auto map_model::get_map() noexcept -> map&
{
  return *m_map.operator->();  // because operator* isn't noexcept
}

auto map_model::get_map() const noexcept -> const map&
{
  return *m_map.operator->();  // because operator* isn't noexcept
}

auto map_model::get() noexcept -> map*
{
  return m_map.get();
}

}  // namespace tactile::core
