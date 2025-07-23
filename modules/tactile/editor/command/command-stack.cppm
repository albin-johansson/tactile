// Copyright (C) 2025 Albin Johansson

export module tactile.editor.command:stack;

export import tactile.core.ext.std;
export import tactile.core;
export import :interfaces;

export namespace tactile::editor {

/// Manages a fixed-size history of editor commands.
class CommandStack final
{
 public:
  /// Creates an empty command stack with the given capacity.
  explicit CommandStack(usize capacity);

  /// Marks the current state as clean.
  void mark_as_clean();

  /// Reverts the most recent command.
  void undo();

  /// Executes the most recently reverted command.
  void redo();

  /// Executes a command and stores it.
  template <std::derived_from<ICommand> T, typename... Args>
  void execute(Args&&... args)
  {
    if (size() == capacity()) {
      _remove_oldest_command();
    }

    _remove_commands_after_cursor();

    // We initially allocate the command on the stack because we won't need to
    // store the command if it gets merged with the most recent command.
    T command {std::forward<Args>(args)...};
    command.redo();

    // If the command can't be merged, store it.
    if (m_commands.empty() || !m_commands.back()->merge(command)) {
      m_commands.push_back(make_unique<T>(std::move(command)));
      _increment_cursor();
    }
    else {
      m_clean_cursor.reset();
      m_has_clean_state = false;
    }
  }

  /// Stores a command without executing it.
  void store(Unique<ICommand> command);

  /// Indicates whether the current state is considered dirty.
  [[nodiscard]]
  auto is_dirty() const -> bool;

  /// Indicates whether a command can be reverted.
  [[nodiscard]]
  auto can_undo() const -> bool;

  /// Indicates whether a command can be re-executed.
  [[nodiscard]]
  auto can_redo() const -> bool;

  /// Returns the size of the stack.
  [[nodiscard]]
  auto size() const -> usize;

  /// Returns the capacity of the stack.
  [[nodiscard]]
  auto capacity() const -> usize;

  /// Returns the index of the command cursor.
  [[nodiscard]]
  auto cursor() const -> Option<usize>;

  /// Returns the index of the clean command cursor.
  [[nodiscard]]
  auto clean_cursor() const -> Option<usize>;

  /// Returns a pointer to the cursor command, if there is one.
  [[nodiscard]]
  auto cursor_command() const -> const ICommand*;

 private:
  usize m_capacity;
  Deque<Unique<ICommand>> m_commands {};
  Option<usize> m_cursor {};
  Option<usize> m_clean_cursor {};
  bool m_has_clean_state {false};

  [[nodiscard]]
  auto _next_cursor_index() const -> usize;

  void _remove_oldest_command();

  void _remove_commands_after_cursor();

  void _increment_cursor();

  void _reset_or_decrement_cursor();
};

}  // namespace tactile::editor
