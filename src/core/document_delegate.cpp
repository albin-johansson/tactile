#include "document_delegate.hpp"

#include <utility>  // move

#include "core/properties/property_delegate.hpp"

namespace Tactile {

DocumentDelegate::DocumentDelegate()
    : mCommandStack{std::make_unique<CommandStack>()}
    , mPropertyContext{std::make_unique<PropertyDelegate>()}
    , mCurrentPropertyContext{mPropertyContext.get()}
{}

DocumentDelegate::DocumentDelegate(std::string name)
    : mCommandStack{std::make_unique<CommandStack>()}
    , mPropertyContext{std::make_unique<PropertyDelegate>(std::move(name))}
    , mCurrentPropertyContext{mPropertyContext.get()}
{}

void DocumentDelegate::Undo()
{
  mCommandStack->Undo();
}

void DocumentDelegate::Redo()
{
  mCommandStack->Redo();
}

void DocumentDelegate::MarkAsClean()
{
  mCommandStack->MarkAsClean();
}

void DocumentDelegate::ResetHistory()
{
  mCommandStack->Clear();
}

void DocumentDelegate::SetCommandCapacity(const usize capacity)
{
  mCommandStack->SetCapacity(capacity);
}

void DocumentDelegate::SetPath(std::filesystem::path path)
{
  mPath = std::move(path);
}

void DocumentDelegate::SetPropertyContext(IPropertyContext* context)
{
  mCurrentPropertyContext = context ? context : mPropertyContext.get();
}

auto DocumentDelegate::GetPropertyContext() -> IPropertyContext*
{
  return mCurrentPropertyContext;
}

auto DocumentDelegate::GetContextName() const -> std::string_view
{
  return mCurrentPropertyContext->GetName();
}

auto DocumentDelegate::CanUndo() const -> bool
{
  return mCommandStack->CanUndo();
}

auto DocumentDelegate::CanRedo() const -> bool
{
  return mCommandStack->CanRedo();
}

auto DocumentDelegate::IsClean() const -> bool
{
  return mCommandStack->IsClean();
}

auto DocumentDelegate::HasPath() const -> bool
{
  return mPath.has_value();
}

auto DocumentDelegate::GetUndoText() const -> std::string
{
  return mCommandStack->GetUndoText();
}

auto DocumentDelegate::GetRedoText() const -> std::string
{
  return mCommandStack->GetRedoText();
}

auto DocumentDelegate::GetPath() const -> std::filesystem::path
{
  return mPath.value();
}

auto DocumentDelegate::GetAbsolutePath() const -> std::filesystem::path
{
  return std::filesystem::absolute(mPath.value());
}

void DocumentDelegate::AddProperty(std::string name, const PropertyType type)
{
  mCurrentPropertyContext->AddProperty(std::move(name), type);
}

void DocumentDelegate::AddProperty(std::string name, const Property& property)
{
  mCurrentPropertyContext->AddProperty(std::move(name), property);
}

void DocumentDelegate::RemoveProperty(const std::string_view name)
{
  mCurrentPropertyContext->RemoveProperty(name);
}

void DocumentDelegate::RenameProperty(const std::string_view oldName,
                                      std::string newName)
{
  mCurrentPropertyContext->RenameProperty(oldName, std::move(newName));
}

void DocumentDelegate::SetProperty(const std::string_view name,
                                   const Property& property)
{
  mCurrentPropertyContext->SetProperty(name, property);
}

void DocumentDelegate::ChangePropertyType(std::string name, const PropertyType type)
{
  mCurrentPropertyContext->ChangePropertyType(std::move(name), type);
}

auto DocumentDelegate::HasProperty(const std::string_view name) const -> bool
{
  return mCurrentPropertyContext->HasProperty(name);
}

auto DocumentDelegate::GetProperty(const std::string_view name) const
    -> const Property&
{
  return mCurrentPropertyContext->GetProperty(name);
}

auto DocumentDelegate::GetProperties() const -> const PropertyMap&
{
  return mCurrentPropertyContext->GetProperties();
}

auto DocumentDelegate::GetPropertyCount() const -> usize
{
  return mCurrentPropertyContext->GetPropertyCount();
}

auto DocumentDelegate::GetName() const -> const std::string&
{
  return mPropertyContext->GetName();
}

}  // namespace Tactile
