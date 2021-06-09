#pragma once

#include <QMenu>  // QMenu

namespace tactile {

class LayerItemContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit LayerItemContextMenu(QWidget* parent);

  void SetVisibilityEnabled(bool enabled);

  void SetRemoveEnabled(bool enabled);

  void SetMoveUpEnabled(bool enabled);

  void SetMoveDownEnabled(bool enabled);

 signals:
  void S_ToggleVisibility();
  void S_ShowProperties();
  void S_MoveLayerUp();
  void S_MoveLayerDown();
  void S_DuplicateLayer();
  void S_RemoveLayer();

 private:
  QAction* mVisibility{};
  QAction* mShowProperties{};
  [[maybe_unused]] QAction* mSep0{};
  QAction* mMoveUp{};
  QAction* mMoveDown{};
  [[maybe_unused]] QAction* mSep1{};
  QAction* mDuplicate{};
  QAction* mRemove{};
};

}  // namespace tactile
