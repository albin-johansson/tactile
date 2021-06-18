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

  void ResetPropertyContext() override;

  void SetPropertyContext(IPropertyManager* manager) override;

  [[nodiscard]] auto GetPropertyContext() -> IPropertyManager* override;

  [[nodiscard]] auto CanUndo() const -> bool override;

  [[nodiscard]] auto CanRedo() const -> bool override;

  [[nodiscard]] auto IsClean() const -> bool override;

  [[nodiscard]] auto HasPath() const -> bool override;

  [[nodiscard]] auto GetUndoText() const -> QString override;

  [[nodiscard]] auto GetRedoText() const -> QString override;

  [[nodiscard]] auto Path() const -> const QFileInfo& override;

  [[nodiscard]] auto AbsolutePath() const -> QString override;

  /// \} End of document API

  /// \name Property API
  /// \{

  void AddProperty(const QString& name, core::PropertyType type) override;

  void AddProperty(const QString& name,
                   const core::Property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name,
                   const core::Property& property) override;

  void ChangePropertyType(const QString& name,
                          core::PropertyType type) override;

  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const core::Property& override;

  [[nodiscard]] auto GetProperty(const QString& name)
      -> core::Property& override;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool override;

  [[nodiscard]] auto PropertyCount() const noexcept -> int override;

  [[nodiscard]] auto GetProperties() const -> const property_map& override;

  [[nodiscard]] auto GetName() const -> QStringView override;

  /// \} End of property API

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
  Unique<CommandStack> mCommandStack;
  Unique<IPropertyManager> mPropertyManager;
  IPropertyManager* mPropertyContext{};
  Maybe<QFileInfo> mPath;
};

}  // namespace tactile::core
