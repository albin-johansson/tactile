#pragma once

#include <filesystem>  // path

#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "commands/command_stack.hpp"
#include "document.hpp"

namespace tactile {

class DocumentDelegate final : public IDocument
{
 public:
  DocumentDelegate();

  /// \name Document API
  /// \{

  void Undo() override;

  void Redo() override;

  void MarkAsClean() override;

  void ResetHistory() override;

  void SetPath(std::filesystem::path path) override;

  void SetPropertyContext(IPropertyContext* context) override;

  [[nodiscard]] auto GetPropertyContext() -> IPropertyContext* override;

  [[nodiscard]] auto CanUndo() const -> bool override;

  [[nodiscard]] auto CanRedo() const -> bool override;

  [[nodiscard]] auto IsClean() const -> bool override;

  [[nodiscard]] auto HasPath() const -> bool override;

  [[nodiscard]] auto GetUndoText() const -> std::string override;

  [[nodiscard]] auto GetRedoText() const -> std::string override;

  [[nodiscard]] auto GetPath() const -> std::filesystem::path override;

  [[nodiscard]] auto GetAbsolutePath() const -> std::filesystem::path override;

  /// \} End of document API

  /// \name Property API
  /// \{

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName,
                      const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

 private:
  Unique<CommandStack> mCommandStack;
  Unique<IPropertyContext> mPropertyContext;
  IPropertyContext* mCurrentPropertyContext{};
  Maybe<std::filesystem::path> mPath;
};

}  // namespace tactile
