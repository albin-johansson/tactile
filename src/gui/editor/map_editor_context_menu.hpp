#pragma once

#include <QMenu>  // QMenu

namespace tactile {

class MapEditorContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit MapEditorContextMenu(QWidget* parent = nullptr);

 signals:
  void S_ShowMapProperties();

 private:
  QAction* mShowProperties{};
};

}  // namespace tactile
