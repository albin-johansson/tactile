#pragma once

#include <QObject>       // QObject
#include <QUndoCommand>  // QUndoCommand
#include <concepts>      // derived_from
#include <utility>       // forward

#include "command_stack.hpp"
#include "document.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

class document_delegate final : public document
{
  Q_OBJECT

 public:
  document_delegate();

  /// \name Document API
  /// \{

  void undo() override;

  void redo() override;

  void mark_as_clean() override;

  void set_path(QFileInfo path) override;

  [[nodiscard]] auto can_undo() const -> bool override;

  [[nodiscard]] auto can_redo() const -> bool override;

  [[nodiscard]] auto is_clean() const -> bool override;

  [[nodiscard]] auto get_undo_text() const -> QString override;

  [[nodiscard]] auto get_redo_text() const -> QString override;

  [[nodiscard]] auto path() const -> const QFileInfo& override;

  /// \}

  /// \name Property API
  /// \{

  void add_property(const QString& name, property::type type) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name,
                    const core::property& property) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const core::property& override;

  [[nodiscard]] auto get_property(const QString& name)
      -> core::property& override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  [[nodiscard]] auto properties() const -> const property_map& override;

  /// \}

  [[nodiscard]] auto get_commands() noexcept -> command_stack*;

  template <std::derived_from<QUndoCommand> T, typename... Args>
  void execute(Args&&... args)
  {
    m_commandStack->template push<T>(std::forward<Args>(args)...);
  }

 private:
  unique<command_stack> m_commandStack;
  unique<property_manager> m_propertyManager;
  QFileInfo m_path;  // TODO make optional? Might improve safety and clarity
};

}  // namespace tactile::core
