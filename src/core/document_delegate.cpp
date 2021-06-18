#include "document_delegate.hpp"

#include <utility>  // move

#include "property_delegate.hpp"

namespace tactile::core {

DocumentDelegate::DocumentDelegate()
    : mCommandStack{std::make_unique<CommandStack>()}
    , mPropertyManager{std::make_unique<PropertyDelegate>(u"Map")}
    , mPropertyContext{mPropertyManager.get()}
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

void DocumentDelegate::ResetPropertyContext()
{
  mPropertyContext = mPropertyManager.get();
}

void DocumentDelegate::SetPropertyContext(not_null<IPropertyManager*> manager)
{
  Q_ASSERT(manager);
  mPropertyContext = manager;
}

auto DocumentDelegate::GetPropertyContext() -> IPropertyManager*
{
  return mPropertyContext;
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
  mPropertyContext->AddProperty(name, type);
  emit S_AddedProperty(name);
}

void DocumentDelegate::AddProperty(const QString& name,
                                   const core::Property& property)
{
  mPropertyContext->AddProperty(name, property);
  emit S_AddedProperty(name);
}

void DocumentDelegate::RemoveProperty(const QString& name)
{
  emit S_AboutToRemoveProperty(name);
  mPropertyContext->RemoveProperty(name);
}

void DocumentDelegate::RenameProperty(const QString& oldName,
                                      const QString& newName)
{
  mPropertyContext->RenameProperty(oldName, newName);
  emit S_RenamedProperty(oldName, newName);
}

void DocumentDelegate::SetProperty(const QString& name,
                                   const core::Property& property)
{
  mPropertyContext->SetProperty(name, property);
  emit S_UpdatedProperty(name);
}

void DocumentDelegate::ChangePropertyType(const QString& name,
                                          const PropertyType type)
{
  mPropertyContext->ChangePropertyType(name, type);
  emit S_ChangedPropertyType(name);
}

auto DocumentDelegate::GetProperty(const QString& name) const
    -> const core::Property&
{
  return mPropertyContext->GetProperty(name);
}

auto DocumentDelegate::GetProperty(const QString& name) -> core::Property&
{
  return mPropertyContext->GetProperty(name);
}

auto DocumentDelegate::HasProperty(const QString& name) const -> bool
{
  return mPropertyContext->HasProperty(name);
}

auto DocumentDelegate::PropertyCount() const noexcept -> int
{
  return mPropertyContext->PropertyCount();
}

auto DocumentDelegate::GetProperties() const -> const property_map&
{
  return mPropertyContext->GetProperties();
}

auto DocumentDelegate::GetName() const -> QStringView
{
  return mPropertyContext->GetName();
}

auto DocumentDelegate::History() noexcept -> CommandStack*
{
  return mCommandStack.get();
}

}  // namespace tactile::core
