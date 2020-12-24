#pragma once

#include <memory>  // unique_ptr

#include "command_stack.hpp"
#include "document.hpp"

namespace tactile::core {

class document_delegate final : public document
{
 public:
  explicit document_delegate(vm::property_model* propertyModel);

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

  [[nodiscard]] auto property_model() const -> vm::property_model* override;

  /// \}

  /// \name Property API
  /// \{

  void add_property(const QString& name, property::type type) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name, const property& property) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const property& override;

  [[nodiscard]] auto get_property(const QString& name) -> property& override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  /// \}

  [[nodiscard]] auto get_commands() noexcept -> command_stack*;

 private:
  std::unique_ptr<command_stack> m_commandStack;
  std::unique_ptr<property_manager> m_propertyManager;
  vm::property_model* m_propertyModel;
  QFileInfo m_path;
};

}  // namespace tactile::core
