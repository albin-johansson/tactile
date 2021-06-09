#include "theme_options_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

ThemeOptionsContextMenu::ThemeOptionsContextMenu(QWidget* parent)
    : QMenu{parent}
    , mRename{addAction(icons::rename(), tr("Rename..."))}
    , mDuplicate{addAction(icons::duplicate(), tr("Duplicate..."))}
    , mSep0{addSeparator()}
    , mExport{addAction(icons::save_as(), tr("Export theme..."))}
    , mSep1{addSeparator()}
    , mReset{addAction(icons::reset(), tr("Reset"))}
    , mSep2{addSeparator()}
    , mRemove{addAction(icons::remove(), tr("Remove"))}
{
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
