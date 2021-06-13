#include "property_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

PropertyContextMenu::PropertyContextMenu(QWidget* parent)
    : QMenu{parent}
    , mCopy{addAction(IconCopy(), tr("Copy"))}
    , mPaste{addAction(IconPaste(), tr("Paste"))}
    , mSep0{addSeparator()}
    , mRename{addAction(IconRename(), tr("Rename"))}
    , mChangeType{addMenu(IconObjectLayer(), tr("Change type"))}
    , mSep1{addSeparator()}
    , mAdd{addAction(IconAdd(), tr("Add new property..."))}
    , mRemove{addAction(IconRemove(), tr("Remove"))}
{
  mCopy->setShortcut(QKeySequence::Copy);
  mPaste->setShortcut(QKeySequence::Paste);
  mRename->setShortcut(QKeySequence::fromString(QStringLiteral(u"CTRL+R")));
  mRemove->setShortcut(QKeySequence::Delete);

  mCopy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  mPaste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  mRename->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  mRemove->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  mTypeString = mChangeType->addAction(tr("string"));
  mTypeInt = mChangeType->addAction(tr("int"));
  mTypeFloat = mChangeType->addAction(tr("float"));
  mTypeBool = mChangeType->addAction(tr("bool"));
  mTypeObject = mChangeType->addAction(tr("object"));
  mTypeColor = mChangeType->addAction(tr("color"));
  mTypeFile = mChangeType->addAction(tr("file"));

  connect(mCopy, &QAction::triggered, this, &PropertyContextMenu::S_Copy);
  connect(mPaste, &QAction::triggered, this, &PropertyContextMenu::S_Paste);
  connect(mRename, &QAction::triggered, this, &PropertyContextMenu::S_Rename);
  connect(mAdd, &QAction::triggered, this, &PropertyContextMenu::S_Add);
  connect(mRemove, &QAction::triggered, this, &PropertyContextMenu::S_Remove);

  connect(mTypeString, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::String);
  });

  connect(mTypeInt, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::Integer);
  });

  connect(mTypeFloat, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::Floating);
  });

  connect(mTypeBool, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::Boolean);
  });

  connect(mTypeFile, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::File);
  });

  connect(mTypeObject, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::Object);
  });

  connect(mTypeColor, &QAction::triggered, [this] {
    emit S_ChangeType(core::PropertyType::Color);
  });

  AddActions(parent);
}

void PropertyContextMenu::AddActions(QWidget* widget)
{
  Q_ASSERT(widget);
  widget->addAction(mCopy);
  widget->addAction(mPaste);
  widget->addAction(mRename);
  widget->addAction(mRemove);
}

void PropertyContextMenu::DisableAll()
{
  mCopy->setEnabled(false);
  mPaste->setEnabled(false);
  mRename->setEnabled(false);
  mChangeType->setEnabled(false);
  mAdd->setEnabled(false);
  mRemove->setEnabled(false);
}

void PropertyContextMenu::SetCurrentType(core::PropertyType type)
{
  for (auto* elem : mChangeType->actions())
  {
    if (!elem->isSeparator())
    {
      elem->setEnabled(true);
    }
  }

  switch (type)
  {
    case core::PropertyType::String:
    {
      mTypeString->setEnabled(false);
      break;
    }
    case core::PropertyType::Integer:
    {
      mTypeInt->setEnabled(false);
      break;
    }
    case core::PropertyType::Floating:
    {
      mTypeFloat->setEnabled(false);
      break;
    }
    case core::PropertyType::Boolean:
    {
      mTypeBool->setEnabled(false);
      break;
    }
    case core::PropertyType::File:
    {
      mTypeFile->setEnabled(false);
      break;
    }
    case core::PropertyType::Color:
    {
      mTypeColor->setEnabled(false);
      break;
    }
    case core::PropertyType::Object:
    {
      mTypeObject->setEnabled(false);
      break;
    }
  }
}

void PropertyContextMenu::SetAddEnabled(const bool enabled)
{
  mAdd->setEnabled(enabled);
}

void PropertyContextMenu::SetRemoveEnabled(const bool enabled)
{
  mRemove->setEnabled(enabled);
}

void PropertyContextMenu::SetRenameEnabled(const bool enabled)
{
  mRename->setEnabled(enabled);
}

void PropertyContextMenu::SetCopyEnabled(const bool enabled)
{
  mCopy->setEnabled(enabled);
}

void PropertyContextMenu::SetPasteEnabled(const bool enabled)
{
  mPaste->setEnabled(enabled);
}

void PropertyContextMenu::SetChangeTypeEnabled(const bool enabled)
{
  mChangeType->setEnabled(enabled);
}

}  // namespace tactile
