// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/command_stack.hpp"

#include "tactile/core/debug/assert.hpp"

namespace tactile {

CommandStack::CommandStack(const std::size_t capacity) :
  m_capacity {capacity}
{}

void CommandStack::mark_as_clean()
{
  m_clean_index = m_current_index;
}

void CommandStack::reset_clean_index()
{
  m_clean_index.reset();
}

void CommandStack::undo()
{
  TACTILE_ASSERT(can_undo());

  m_commands.at(m_current_index.value())->undo();
  _reset_or_decrease_current_index();
}

void CommandStack::redo()
{
  TACTILE_ASSERT(can_redo());

  m_commands.at(_get_next_command_index())->redo();
  _increase_current_index();
}

void CommandStack::_store(Unique<ICommand> cmd)
{
  if (size() == capacity()) {
    _remove_oldest_command();
  }

  _remove_commands_after_current_index();
  _increase_current_index();

  m_commands.push_back(std::move(cmd));
}

void CommandStack::set_capacity(const std::size_t capacity)
{
  m_capacity = capacity;

  const auto command_count = size();
  if (command_count > m_capacity) {
    const auto excess_count = command_count - m_capacity;
    for (std::size_t i = 0; i < excess_count; ++i) {
      _remove_oldest_command();
    }
  }
}

auto CommandStack::is_clean() const -> bool
{
  return m_commands.empty() || (m_clean_index == m_current_index);
}

auto CommandStack::can_undo() const -> bool
{
  return !m_commands.empty() && m_current_index.has_value();
}

auto CommandStack::can_redo() const -> bool
{
  return (!m_commands.empty() && !m_current_index.has_value()) ||
         (!m_commands.empty() && m_current_index < m_commands.size() - 1);
}

auto CommandStack::size() const -> std::size_t
{
  return m_commands.size();
}

auto CommandStack::capacity() const -> std::size_t
{
  return m_capacity;
}

auto CommandStack::index() const -> Optional<std::size_t>
{
  return m_current_index;
}

auto CommandStack::clean_index() const -> Optional<std::size_t>
{
  return m_clean_index;
}

void CommandStack::_remove_oldest_command()
{
  TACTILE_ASSERT(!m_commands.empty());

  m_commands.front()->dispose();
  m_commands.pop_front();
  _reset_or_decrease_current_index();
  _reset_or_decrease_clean_index();
}

void CommandStack::_remove_commands_after_current_index()
{
  const auto start_index = _get_next_command_index();

  // If we have a clean index, and there are undone commands when another
  // command is pushed, then the clean index becomes invalidated.
  if (m_clean_index >= start_index) {
    m_clean_index.reset();
  }

  const auto command_count = m_commands.size();
  for (auto cmd_index = start_index; cmd_index < command_count; ++cmd_index) {
    m_commands.back()->dispose();
    m_commands.pop_back();
  }
}

void CommandStack::_reset_or_decrease_clean_index()
{
  if (m_clean_index.has_value()) {
    if (m_clean_index == 0) {
      m_clean_index = kNone;
    }
    else {
      m_clean_index = *m_clean_index - 1;
    }
  }
}

void CommandStack::_reset_or_decrease_current_index()
{
  if (m_current_index.has_value()) {
    if (m_current_index == 0) {
      m_current_index = kNone;
    }
    else {
      m_current_index = *m_current_index - 1;
    }
  }
}

void CommandStack::_increase_current_index()
{
  m_current_index = _get_next_command_index();
}

auto CommandStack::_get_next_command_index() const -> std::size_t
{
  return m_current_index.has_value() ? *m_current_index + 1 : 0;
}

}  // namespace tactile
