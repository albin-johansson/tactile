#include "document_delegate.hpp"

#include <utility>  // move

#include "property_delegate.hpp"

namespace tactile::core {

DocumentDelegate::DocumentDelegate()
    : mCommandStack{std::make_unique<CommandStack>()}
    , mPropertyManager{std::make_unique<property_delegate>()}
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

void DocumentDelegate::add_property(const QString& name,
                                    const core::property_type type)
{
  mPropertyManager->add_property(name, type);
  emit S_AddedProperty(name);
}

void DocumentDelegate::add_property(const QString& name,
                                    const core::property& property)
{
  mPropertyManager->add_property(name, property);
  emit S_AddedProperty(name);
}

void DocumentDelegate::remove_property(const QString& name)
{
  emit S_AboutToRemoveProperty(name);
  mPropertyManager->remove_property(name);
}

void DocumentDelegate::rename_property(const QString& oldName,
                                       const QString& newName)
{
  mPropertyManager->rename_property(oldName, newName);
  emit S_RenamedProperty(oldName, newName);
}

void DocumentDelegate::set_property(const QString& name,
                                    const core::property& property)
{
  mPropertyManager->set_property(name, property);
  emit S_UpdatedProperty(name);
}

void DocumentDelegate::change_property_type(const QString& name,
                                            const core::property_type type)
{
  mPropertyManager->change_property_type(name, type);
  emit S_ChangedPropertyType(name);
}

auto DocumentDelegate::get_property(const QString& name) const
    -> const core::property&
{
  return mPropertyManager->get_property(name);
}

auto DocumentDelegate::get_property(const QString& name) -> core::property&
{
  return mPropertyManager->get_property(name);
}

auto DocumentDelegate::has_property(const QString& name) const -> bool
{
  return mPropertyManager->has_property(name);
}

auto DocumentDelegate::property_count() const noexcept -> int
{
  return mPropertyManager->property_count();
}

auto DocumentDelegate::properties() const -> const property_map&
{
  return mPropertyManager->properties();
}

auto DocumentDelegate::History() noexcept -> CommandStack*
{
  return mCommandStack.get();
}

}  // namespace tactile::core
