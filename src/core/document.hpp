#pragma once

#include <QFileInfo>  // QFileInfo
#include <QObject>    // QObject
#include <QString>    // QString

#include "not_null.hpp"
#include "property_manager.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

/**
 * \class ADocument
 *
 * \brief Represents a persistent document, e.g. for tilemaps.
 *
 * \see MapDocument
 *
 * \since 0.2.0
 *
 * \headerfile document.hpp
 */
class ADocument : public QObject, public IPropertyManager
{
  Q_OBJECT

 public:
  explicit ADocument(QObject* parent = nullptr) : QObject{parent}
  {}

  ~ADocument() noexcept override = default;

  /**
   * \brief Reverts the effects of the most recent command.
   *
   * \note This function has no effect if there is no undoable command.
   *
   * \since 0.2.0
   */
  virtual void Undo() = 0;

  /**
   * \brief Executes the most recently reverted command.
   *
   * \note This function has no effect if there is no redoable command.
   *
   * \since 0.2.0
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
   *
   * \since 0.2.0
   */
  virtual void MarkAsClean() = 0;

  /**
   * \brief Clears the entire undo/redo history associated with the document.
   *
   * \since 0.2.0
   */
  virtual void ResetHistory() = 0;

  /**
   * \brief Sets the file path associated with the document.
   *
   * \param path the file path of the map document.
   *
   * \since 0.2.0
   */
  virtual void SetPath(QFileInfo path) = 0;

  virtual void ResetPropertyContext() = 0;

  virtual void SetPropertyContext(not_null<IPropertyManager*> manager) = 0;

  [[nodiscard]] virtual auto GetPropertyContext() -> IPropertyManager* = 0;

  /**
   * \brief Indicates whether or not there is an undoable command.
   *
   * \return `true` if there is an undoable command; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto CanUndo() const -> bool = 0;

  /**
   * \brief Indicates whether or not there is an redoable command.
   *
   * \return `true` if there is an redoable command; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto CanRedo() const -> bool = 0;

  /**
   * \brief Indicates whether or not the command state is clean.
   *
   * \return `true` if the command state is clean; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto IsClean() const -> bool = 0;

  /**
   * \brief Indicates whether or not there is a file path associated with the
   * document.
   *
   * \return `true` if there is a file path associated with the document;
   * `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto HasPath() const -> bool = 0;

  /**
   * \brief Returns the text associated with the currently undoable command.
   *
   * \return the text associated with the currently undoable command.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto GetUndoText() const -> QString = 0;

  /**
   * \brief Returns the text associated with the currently redoable command.
   *
   * \return the text associated with the currently redoable command.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto GetRedoText() const -> QString = 0;

  /**
   * \brief Returns the file path associated with the document.
   *
   * \return the file path associated with the  document.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto Path() const -> const QFileInfo& = 0;

  /**
   * \brief Returns the absolute file path associated with the document.
   *
   * \pre There must be a file path associated with the document.
   *
   * \return the absolute file path associated with the document.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto AbsolutePath() const -> QString = 0;
};

}  // namespace tactile::core
