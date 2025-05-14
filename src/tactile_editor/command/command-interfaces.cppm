// Copyright (C) 2025 Albin Johansson

export module tactile.editor.command:interfaces;

export namespace tactile::editor {

/// Interface for editor commands.
class ICommand
{
 protected:
  ICommand() = default;

  ICommand(ICommand&&) noexcept = default;

  ICommand(const ICommand&) = default;

  auto operator=(ICommand&&) noexcept -> ICommand& = default;

  auto operator=(const ICommand&) -> ICommand& = default;

 public:
  virtual ~ICommand() noexcept = default;

  /// Executes the command.
  virtual void redo() = 0;

  /// Reverts the effects of the redo function.
  virtual void undo() = 0;

  /// Attempts to merge another command into this command.
  ///
  /// This function can be used to combine consecutive high-frequency commands of
  /// the same type. For example, the user changing a color property via a color
  /// picker may trigger many "update property" events in a short amount of time.
  /// However, most users would consider the color change as a single action, so
  /// calling undo should revert the color to the initial value, skipping any
  /// intermediate values.
  [[nodiscard]]
  virtual auto merge([[maybe_unused]] const ICommand& other) -> bool
  {
    return false;
  }
};

}  // namespace tactile::editor
