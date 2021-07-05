#pragma once

#include <concepts>    // derived_from
#include <filesystem>  // path
#include <string>      // string
#include <utility>     // forward

#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "core/commands/command_stack.hpp"
#include "core/document.hpp"

namespace Tactile {

class DocumentDelegate final : public IDocument
{
 public:
  DocumentDelegate();

  explicit DocumentDelegate(std::string name);

  /// \name Document API
  /// \{

  void Undo() override;

  void Redo() override;

  void MarkAsClean() override;

  void ResetHistory() override;

  void SetPath(std::filesystem::path path) override;

  void SetPropertyContext(IPropertyContext* context) override;

  [[nodiscard]] auto GetPropertyContext() -> IPropertyContext* override;

  [[nodiscard]] auto GetContextName() const -> std::string_view override;

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

  void RenameProperty(std::string_view oldName, const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

  /// \name Command API
  /// \{

  template <std::derived_from<ACommand> T, typename... Args>
  void Execute(Args&&... args)
  {
    mCommandStack->Push<T>(std::forward<Args>(args)...);
  }

  /// \} End of command API

 private:
  Unique<CommandStack> mCommandStack;
  Unique<IPropertyContext> mPropertyContext;
  IPropertyContext* mCurrentPropertyContext{};
  Maybe<std::filesystem::path> mPath;
};

}  // namespace Tactile
