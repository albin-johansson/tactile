#include "theme_options_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

ThemeOptionsContextMenu::ThemeOptionsContextMenu(QWidget* parent)
    : QMenu{parent}
    , mRename{addAction(IconRename(), tr("Rename..."))}
    , mDuplicate{addAction(IconDuplicate(), tr("Duplicate..."))}
    , mSep0{addSeparator()}
    , mExport{addAction(IconSaveAs(), tr("Export theme..."))}
    , mSep1{addSeparator()}
    , mReset{addAction(IconReset(), tr("Reset"))}
    , mSep2{addSeparator()}
    , mRemove{addAction(IconRemove(), tr("Remove"))}
{
  setObjectName(QStringLiteral(u"ThemeOptionsContextMenu"));

  // clang-format off
  connect(mRename, &QAction::triggered,
          this, &ThemeOptionsContextMenu::S_RenameTheme);

  connect(mDuplicate, &QAction::triggered,
          this, &ThemeOptionsContextMenu::S_DuplicateTheme);

  connect(mExport, &QAction::triggered,
          this, &ThemeOptionsContextMenu::S_ExportTheme);

  connect(mReset, &QAction::triggered,
          this, &ThemeOptionsContextMenu::S_ResetTheme);

  connect(mRemove, &QAction::triggered,
          this, &ThemeOptionsContextMenu::S_RemoveTheme);
  // clang-format on
}

void ThemeOptionsContextMenu::SetRenameEnabled(const bool enabled)
{
  mRename->setEnabled(enabled);
}

void ThemeOptionsContextMenu::SetResetEnabled(const bool enabled)
{
  mReset->setEnabled(enabled);
}

void ThemeOptionsContextMenu::SetRemoveEnabled(const bool enabled)
{
  mRemove->setEnabled(enabled);
}

}  // namespace tactile
