#include "document_delegate.hpp"

#include <utility>  // move

#include "property_delegate.hpp"
#include "tactile_error.hpp"

namespace tactile::core {

document_delegate::document_delegate()
    : m_commandStack{std::make_unique<command_stack>()}
    , m_propertyManager{std::make_unique<property_delegate>()}
{
  m_commandStack->setUndoLimit(100);
}

void document_delegate::undo()
{
  m_commandStack->undo();
}

void document_delegate::redo()
{
  m_commandStack->redo();
}

void document_delegate::mark_as_clean()
{
  m_commandStack->setClean();
}

void document_delegate::set_path(QFileInfo path)
{
  m_path = std::move(path);
}

auto document_delegate::can_undo() const -> bool
{
  return m_commandStack->canUndo();
}

auto document_delegate::can_redo() const -> bool
{
  return m_commandStack->canRedo();
}

auto document_delegate::is_clean() const -> bool
{
  return m_commandStack->isClean();
}

auto document_delegate::get_undo_text() const -> QString
{
  return m_commandStack->undoText();
}

auto document_delegate::get_redo_text() const -> QString
{
  return m_commandStack->redoText();
}

auto document_delegate::path() const -> const QFileInfo&
{
  return m_path;
}

void document_delegate::notify_property_added(const QString& name)
{
  emit added_property(name);
}

void document_delegate::notify_property_removed(const QString& name)
{
  emit removed_property(name);
}

void document_delegate::add_property(const QString& name,
                                     const property::type type)
{
  m_propertyManager->add_property(name, type);
}

void document_delegate::remove_property(const QString& name)
{
  m_propertyManager->remove_property(name);
}

void document_delegate::rename_property(const QString& oldName,
                                        const QString& newName)
{
  m_propertyManager->rename_property(oldName, newName);
}

void document_delegate::set_property(const QString& name,
                                     const core::property& property)
{
  m_propertyManager->set_property(name, property);
}

auto document_delegate::get_property(const QString& name) const
    -> const core::property&
{
  return m_propertyManager->get_property(name);
}

auto document_delegate::get_property(const QString& name) -> core::property&
{
  return m_propertyManager->get_property(name);
}

auto document_delegate::property_count() const noexcept -> int
{
  return m_propertyManager->property_count();
}

auto document_delegate::properties() const -> const property_map&
{
  return m_propertyManager->properties();
}

auto document_delegate::get_commands() noexcept -> command_stack*
{
  return m_commandStack.get();
}

}  // namespace tactile::core
