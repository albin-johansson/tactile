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

class DocumentDelegate final : public ADocument
{
  Q_OBJECT

 public:
  DocumentDelegate();

  /// \name Document API
  /// \{

  void Undo() override;

  void Redo() override;

  void MarkAsClean() override;

  void ResetHistory() override;

  void SetPath(QFileInfo path) override;

  [[nodiscard]] auto CanUndo() const -> bool override;

  [[nodiscard]] auto CanRedo() const -> bool override;

  [[nodiscard]] auto IsClean() const -> bool override;

  [[nodiscard]] auto HasPath() const -> bool override;

  [[nodiscard]] auto GetUndoText() const -> QString override;

  [[nodiscard]] auto GetRedoText() const -> QString override;

  [[nodiscard]] auto Path() const -> const QFileInfo& override;

  [[nodiscard]] auto AbsolutePath() const -> QString override;

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

  [[nodiscard]] auto History() noexcept -> CommandStack*;

  template <std::derived_from<QUndoCommand> T, typename... Args>
  void Execute(Args&&... args)
  {
    mCommandStack->template Push<T>(std::forward<Args>(args)...);
  }

 signals:
  void S_AddedProperty(const QString& name);
  void S_AboutToRemoveProperty(const QString& name);
  void S_UpdatedProperty(const QString& name);
  void S_ChangedPropertyType(const QString& name);
  void S_RenamedProperty(const QString& oldName, const QString& newName);

 private:
  unique<CommandStack> mCommandStack;
  unique<property_manager> mPropertyManager;
  maybe<QFileInfo> mPath;
};

}  // namespace tactile::core
