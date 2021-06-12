#include "document_delegate.hpp"

#include <utility>  // move

#include "property_delegate.hpp"

namespace tactile::core {

DocumentDelegate::DocumentDelegate()
    : mCommandStack{std::make_unique<CommandStack>()}
    , mPropertyManager{std::make_unique<PropertyDelegate>()}
{
  mCommandStack->setUndoLimit(100);
}

void DocumentDelegate::Undo()
{
  mCommandStack->undo();
}

void DocumentDelegate::Redo()
{
  mCommandStack->redo();
}

void DocumentDelegate::MarkAsClean()
{
  mCommandStack->setClean();
}

void DocumentDelegate::ResetHistory()
{
  mCommandStack->clear();
}

void DocumentDelegate::SetPath(QFileInfo path)
{
  mPath = std::move(path);
}

auto DocumentDelegate::CanUndo() const -> bool
{
  return mCommandStack->canUndo();
}

auto DocumentDelegate::CanRedo() const -> bool
{
  return mCommandStack->canRedo();
}

auto DocumentDelegate::IsClean() const -> bool
{
  return mCommandStack->isClean();
}

auto DocumentDelegate::HasPath() const -> bool
{
  return mPath && mPath->exists();
}

auto DocumentDelegate::GetUndoText() const -> QString
{
  return mCommandStack->undoText();
}

auto DocumentDelegate::GetRedoText() const -> QString
{
  return mCommandStack->redoText();
}

auto DocumentDelegate::Path() const -> const QFileInfo&
{
  Q_ASSERT(HasPath());
  return mPath.value();
}

auto DocumentDelegate::AbsolutePath() const -> QString
{
  Q_ASSERT(HasPath());
  return mPath->absoluteFilePath();
}

void DocumentDelegate::AddProperty(const QString& name, const PropertyType type)
{
  mPropertyManager->AddProperty(name, type);
  emit S_AddedProperty(name);
}

void DocumentDelegate::AddProperty(const QString& name,
                                   const core::Property& property)
{
  mPropertyManager->AddProperty(name, property);
  emit S_AddedProperty(name);
}

void DocumentDelegate::RemoveProperty(const QString& name)
{
  emit S_AboutToRemoveProperty(name);
  mPropertyManager->RemoveProperty(name);
}

void DocumentDelegate::RenameProperty(const QString& oldName,
                                      const QString& newName)
{
  mPropertyManager->RenameProperty(oldName, newName);
  emit S_RenamedProperty(oldName, newName);
}

void DocumentDelegate::SetProperty(const QString& name,
                                   const core::Property& property)
{
  mPropertyManager->SetProperty(name, property);
  emit S_UpdatedProperty(name);
}

void DocumentDelegate::ChangePropertyType(const QString& name,
                                          const PropertyType type)
{
  mPropertyManager->ChangePropertyType(name, type);
  emit S_ChangedPropertyType(name);
}

auto DocumentDelegate::GetProperty(const QString& name) const
    -> const core::Property&
{
  return mPropertyManager->GetProperty(name);
}

auto DocumentDelegate::GetProperty(const QString& name) -> core::Property&
{
  return mPropertyManager->GetProperty(name);
}

auto DocumentDelegate::HasProperty(const QString& name) const -> bool
{
  return mPropertyManager->HasProperty(name);
}

auto DocumentDelegate::PropertyCount() const noexcept -> int
{
  return mPropertyManager->PropertyCount();
}

auto DocumentDelegate::GetProperties() const -> const property_map&
{
  return mPropertyManager->GetProperties();
}

auto DocumentDelegate::History() noexcept -> CommandStack*
{
  return mCommandStack.get();
}

}  // namespace tactile::core
