// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/command/command_stack.hpp"

#include <stdexcept>
#include <utility>

namespace tactile::editor {

CommandStack::CommandStack(const isize capacity)
  : m_capacity {capacity}
{
  if (m_capacity <= 0) {
    throw std::invalid_argument {"[CommandStack]: bad capacity"};
  }
}

void CommandStack::mark_as_clean()
{
  m_clean_command_index = m_cursor_command_index;
  m_has_clean_state = true;
}

void CommandStack::undo()
{
  const auto command_index = get_undo_command_index();
  if (!command_index.has_value()) {
    throw std::runtime_error {"[CommandStack::undo]: cannot undo"};
  }

  m_commands.at(*command_index)->undo();
  --m_cursor_command_index;
}

void CommandStack::redo()
{
  const auto command_index = get_redo_command_index();
  if (!command_index.has_value()) {
    throw std::runtime_error {"[CommandStack::redo]: cannot redo"};
  }

  m_commands.at(*command_index)->redo();
  ++m_cursor_command_index;
}

void CommandStack::store(Unique<ICommand> command)
{
  if (command == nullptr) {
    throw std::invalid_argument {"[CommandStack::store]: null command"};
  }

  if (size() == capacity()) {
    remove_oldest_command();
  }

  remove_commands_after_cursor();

  m_commands.push_back(std::move(command));
  ++m_cursor_command_index;
}

auto CommandStack::is_clean() const -> bool
{
  return m_has_clean_state && m_cursor_command_index == m_clean_command_index;
}

auto CommandStack::can_undo() const -> bool
{
  return get_undo_command_index().has_value();
}

auto CommandStack::can_redo() const -> bool
{
  return get_redo_command_index().has_value();
}

auto CommandStack::size() const -> isize
{
  return std::ssize(m_commands);
}

auto CommandStack::capacity() const -> isize
{
  return m_capacity;
}

auto CommandStack::get_cursor_command() const -> const ICommand*
{
  return m_cursor_command_index >= 0
             ? m_commands.at(static_cast<usize>(m_cursor_command_index)).get()
             : nullptr;
}

void CommandStack::remove_oldest_command()
{
  m_commands.pop_front();
  --m_cursor_command_index;
}

void CommandStack::remove_commands_after_cursor()
{
  if (m_clean_command_index > m_cursor_command_index) {
    reset_clean_state();
  }

  const auto command_count = size();
  for (auto index = m_cursor_command_index + 1; index < command_count; ++index) {
    m_commands.pop_back();
  }
}

void CommandStack::reset_clean_state()
{
  m_clean_command_index = -1;
  m_has_clean_state = false;
}

auto CommandStack::get_undo_command_index() const -> Option<usize>
{
  return m_cursor_command_index >= 0
             ? make_optional(static_cast<usize>(m_cursor_command_index))
             : kNone;
}

auto CommandStack::get_redo_command_index() const -> Option<usize>
{
  const auto index = m_cursor_command_index + 1;
  return index < size() ? make_optional(static_cast<usize>(index)) : kNone;
}

}  // namespace tactile::editor
