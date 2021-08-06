#pragma once

#include <filesystem>   // path
#include <string>       // string
#include <string_view>  // string_view

#include "properties/property_context.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class IDocument : public IPropertyContext
{
 public:
  /**
   * \brief Reverts the effects of the most recent command.
   *
   * \note This function has no effect if there is no undoable command.
   */
  virtual void Undo() = 0;

  /**
   * \brief Executes the most recently reverted command.
   *
   * \note This function has no effect if there is no redoable command.
   */
  virtual void Redo() = 0;

  /**
   * \brief Marks the command stack state as "clean".
   *
   * \details Marking the state of the document as clean means that this is
   * the default state, used to prevent saving a document unnecessarily when no
   * effective change has been made to it.
   *
   * \note This function should be called every time the document is saved.
   */
  virtual void MarkAsClean() = 0;

  /// \brief Clears the entire undo/redo history associated with the document.
  virtual void ResetHistory() = 0;

  /**
   * \brief Sets the file path associated with the document.
   *
   * \param path the associated file path.
   */
  virtual void SetPath(std::filesystem::path path) = 0;

  /**
   * \brief Sets the currently active property context.
   *
   * \details A null pointer can be supplied to this function to reset the
   * property context, i.e. make the document itself the active property
   * context.
   *
   * \param context the new property context.
   */
  virtual void SetPropertyContext(IPropertyContext* context) = 0;

  /**
   * \brief Returns the currently active property context.
   *
   * \details Documents have associated properties, but they can also host
   * associated components that also support properties. For example, tilemaps
   * feature properties but so do the objects present in the tilemaps. The
   * property "context" is simply the currently active property manager.
   *
   * \return the active property context, might be null if the document itself
   * is the current property context.
   */
  [[nodiscard]] virtual auto GetPropertyContext() -> IPropertyContext* = 0;

  /**
   * \brief Returns the name of the current property context.
   *
   * \return the current property context name.
   */
  [[nodiscard]] virtual auto GetContextName() const -> std::string_view = 0;

  /**
   * \brief Indicates whether or not there is a currently undoable command.
   *
   * \return `true` if undo is possible; `false` otherwise.
   */
  [[nodiscard]] virtual auto CanUndo() const -> bool = 0;

  /**
   * \brief Indicates whether or not there is a currently redoable command.
   *
   * \return `true` if redo is possible; `false` otherwise.
   */
  [[nodiscard]] virtual auto CanRedo() const -> bool = 0;

  /**
   * \brief Indicates whether or not the command state is clean.
   *
   * \return `true` if the command state is clean; `false` otherwise.
   */
  [[nodiscard]] virtual auto IsClean() const -> bool = 0;

  /**
   * \brief Indicates whether or not there is a file path associated with the
   * document.
   *
   * \return `true` if there is an associated file path; `false` otherwise.
   */
  [[nodiscard]] virtual auto HasPath() const -> bool = 0;

  /**
   * \brief Returns the text associated with the currently undoable command.
   *
   * \return the current undoable command text.
   */
  [[nodiscard]] virtual auto GetUndoText() const -> std::string = 0;

  /**
   * \brief Returns the text associated with the currently redoable command.
   *
   * \return the current redoable command text.
   */
  [[nodiscard]] virtual auto GetRedoText() const -> std::string = 0;

  /// \brief Returns the file path associated with the document.
  [[nodiscard]] virtual auto GetPath() const -> std::filesystem::path = 0;

  /**
   * \brief Returns the absolute file path associated with the document.
   *
   * \pre There must be an assigned file path.
   *
   * \return the absolute file path.
   */
  [[nodiscard]] virtual auto GetAbsolutePath() const -> std::filesystem::path = 0;
};

/// \} End of group core

}  // namespace Tactile
