#include "layer_item_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

LayerItemContextMenu::LayerItemContextMenu(QWidget* parent)
    : QMenu{parent}
    , mVisibility{addAction(IconVisible(), tr("Toggle visibility"))}
    , mShowProperties{addAction(IconProperties(), tr("Show properties..."))}
    , mSep0{addSeparator()}
    , mMoveUp{addAction(IconMoveUp(), tr("Move layer up"))}
    , mMoveDown{addAction(IconMoveDown(), tr("Move layer down"))}
    , mSep1{addSeparator()}
    , mDuplicate{addAction(IconDuplicate(), tr("Duplicate layer"))}
    , mRemove{addAction(IconRemove(), tr("Remove layer"))}
{
  setObjectName(QStringLiteral(u"LayerItemContextMenu"));

  // clang-format off
  connect(mVisibility, &QAction::triggered, this, &LayerItemContextMenu::S_ToggleVisibility);
  connect(mShowProperties, &QAction::triggered, this, &LayerItemContextMenu::S_ShowProperties);
  connect(mMoveUp, &QAction::triggered, this, &LayerItemContextMenu::S_MoveLayerUp);
  connect(mMoveDown, &QAction::triggered, this, &LayerItemContextMenu::S_MoveLayerDown);
  connect(mDuplicate, &QAction::triggered, this, &LayerItemContextMenu::S_DuplicateLayer);
  connect(mRemove, &QAction::triggered, this, &LayerItemContextMenu::S_RemoveLayer);
  // clang-format on

  mDuplicate->setShortcut(QKeySequence::fromString(QStringLiteral(u"CTRL+D")));
  mRemove->setShortcut(QKeySequence::Delete);
  mMoveUp->setShortcut(QKeySequence::fromString(QStringLiteral(u"SHIFT+UP")));
  mMoveDown->setShortcut(
      QKeySequence::fromString(QStringLiteral(u"SHIFT+DOWN")));

  mDuplicate->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  mRemove->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  mMoveUp->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  mMoveDown->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  Q_ASSERT(parent);
  parent->addAction(mDuplicate);
  parent->addAction(mRemove);
  parent->addAction(mMoveUp);
  parent->addAction(mMoveDown);
}

void LayerItemContextMenu::SetVisibilityEnabled(const bool enabled)
{
  mVisibility->setEnabled(enabled);
}

void LayerItemContextMenu::SetRemoveEnabled(const bool enabled)
{
  mRemove->setEnabled(enabled);
}

void LayerItemContextMenu::SetMoveUpEnabled(const bool enabled)
{
  mMoveUp->setEnabled(enabled);
}

void LayerItemContextMenu::SetMoveDownEnabled(const bool enabled)
{
  mMoveDown->setEnabled(enabled);
}

}  // namespace tactile
