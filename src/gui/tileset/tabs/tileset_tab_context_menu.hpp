#pragma once

#include <QMenu>  // QMenu

namespace tactile {

class TilesetTabContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit TilesetTabContextMenu(QWidget* parent = nullptr);

  void SetTabIndex(int index);

 signals:
  void S_Rename(int index);
  void S_Remove(int index);
  void S_ShowProperties(int index);

 private:
  QAction* mRename{};
  [[maybe_unused]] QAction* mSep0{};
  QAction* mRemove{};
  [[maybe_unused]] QAction* mSep1{};
  QAction* mShowProperties{};
  int mIndex{-1};
};

}  // namespace tactile
