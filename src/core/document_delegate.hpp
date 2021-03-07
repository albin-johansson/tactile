#pragma once

#include <QObject>       // QObject
#include <QUndoCommand>  // QUndoCommand
#include <concepts>      // derived_from
#include <utility>       // forward

#include "command_stack.hpp"
#include "document.hpp"
#include "maybe.hpp"
#include "property_model.hpp"
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

  void reset_history() override;

  void set_path(QFileInfo path) override;

  [[nodiscard]] auto can_undo() const -> bool override;

  [[nodiscard]] auto can_redo() const -> bool override;

  [[nodiscard]] auto is_clean() const -> bool override;

  [[nodiscard]] auto has_path() const -> bool override;

  [[nodiscard]] auto get_undo_text() const -> QString override;

  [[nodiscard]] auto get_redo_text() const -> QString override;

  [[nodiscard]] auto path() const -> const QFileInfo& override;

  [[nodiscard]] auto absolute_path() const -> QString override;

  /// \}

  /// \name Property API
  /// \{

  void add_property(const QString& name, core::property_type type) override;

  void add_property(const QString& name,
                    const core::property& property) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name,
                    const core::property& property) override;

  void change_property_type(const QString& name,
                            core::property_type type) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const core::property& override;

  [[nodiscard]] auto get_property(const QString& name)
      -> core::property& override;

  [[nodiscard]] auto has_property(const QString& name) const -> bool override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  [[nodiscard]] auto properties() const -> const property_map& override;

  /// \}

  [[nodiscard]] auto get_commands() noexcept -> command_stack*;

  template <std::derived_from<QUndoCommand> T, typename... Args>
  void execute(Args&&... args)
  {
    m_commandStack->template push<T>(std::forward<Args>(args)...);
  }

 signals:
  void added_property(const QString& name);
  void about_to_remove_property(const QString& name);
  void updated_property(const QString& name);
  void changed_property_type(const QString& name);
  void renamed_property(const QString& oldName, const QString& newName);

 private:
  unique<command_stack> m_commandStack;
  unique<property_manager> m_propertyManager;
  maybe<QFileInfo> m_path;
};

}  // namespace tactile::core
