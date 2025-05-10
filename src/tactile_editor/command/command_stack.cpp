// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module tactile_editor.command;

import std;

namespace tactile::editor {

CommandStack::CommandStack(const usize capacity)
  : m_capacity {capacity}
{
  if (m_capacity < 1) {
    throw std::invalid_argument {"invalid command stack capacity"};
  }
}

void CommandStack::mark_as_clean()
{
  m_clean_cursor = m_cursor;
  m_has_clean_state = true;
}

void CommandStack::undo()
{
  if (!can_undo()) {
    throw std::runtime_error {"couldn't undo command"};
  }

  const auto& command = m_commands.at(m_cursor.value());
  command->undo();

  _reset_or_decrement_cursor();
}

void CommandStack::redo()
{
  if (!can_redo()) {
    throw std::runtime_error {"couldn't redo command"};
  }

  const auto& command = m_commands.at(_next_cursor_index());
  command->redo();

  _increment_cursor();
}

void CommandStack::store(Unique<ICommand> command)
{
  if (size() == capacity()) {
    _remove_oldest_command();
  }

  _remove_commands_after_cursor();

  m_commands.push_back(std::move(command));
  _increment_cursor();
}

auto CommandStack::is_dirty() const -> bool
{
  return !m_has_clean_state || m_cursor != m_clean_cursor;
}

auto CommandStack::can_undo() const -> bool
{
  return !m_commands.empty() && m_cursor.has_value();
}

auto CommandStack::can_redo() const -> bool
{
  return !m_commands.empty() && m_cursor < size() - 1uz;
}

auto CommandStack::size() const -> usize
{
  return m_commands.size();
}

auto CommandStack::capacity() const -> usize
{
  return m_capacity;
}

auto CommandStack::cursor() const -> Option<usize>
{
  return m_cursor;
}

auto CommandStack::clean_cursor() const -> Option<usize>
{
  return m_clean_cursor;
}

auto CommandStack::cursor_command() const -> const ICommand*
{
  return m_cursor.has_value() ? m_commands.at(*m_cursor).get() : nullptr;
}

auto CommandStack::_next_cursor_index() const -> usize
{
  return m_cursor.has_value() ? *m_cursor + 1uz : 0uz;
}

void CommandStack::_remove_oldest_command()
{
  m_commands.pop_front();
  _reset_or_decrement_cursor();
}

void CommandStack::_remove_commands_after_cursor()
{
  const auto start_index = _next_cursor_index();

  if (m_clean_cursor >= start_index) {
    m_clean_cursor.reset();
    m_has_clean_state = false;
  }

  const auto command_count = size();
  for (auto index = start_index; index < command_count; ++index) {
    m_commands.pop_back();
  }
}

void CommandStack::_increment_cursor()
{
  m_cursor = _next_cursor_index();
}

void CommandStack::_reset_or_decrement_cursor()
{
  if (!m_cursor.has_value()) {
    return;
  }

  if (m_cursor == 0) {
    m_cursor.reset();
  }
  else {
    m_cursor = *m_cursor - 1;
  }
}

}  // namespace tactile::editor
