#pragma once

#include <QMenu>  // QMenu

namespace tactile {

class ThemeOptionsContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit ThemeOptionsContextMenu(QWidget* parent = nullptr);

  void SetRenameEnabled(bool enabled);

  void SetResetEnabled(bool enabled);

  void SetRemoveEnabled(bool enabled);

 signals:
  void S_RenameTheme();
  void S_DuplicateTheme();
  void S_ExportTheme();
  void S_ResetTheme();
  void S_RemoveTheme();

 private:
  QAction* mRename{};
  QAction* mDuplicate{};
  [[maybe_unused]] QAction* mSep0{};
  QAction* mExport{};
  [[maybe_unused]] QAction* mSep1{};
  QAction* mReset{};
  [[maybe_unused]] QAction* mSep2{};
  QAction* mRemove{};
};

}  // namespace tactile
