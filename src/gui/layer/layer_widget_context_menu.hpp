#pragma once

#include <QMenu>  // QMenu

namespace tactile {

class AddLayerContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit AddLayerContextMenu(QWidget* parent = nullptr);

 signals:
  void S_AddTileLayer();
  void S_AddObjectLayer();

 private:
  QAction* mTileLayer{};
  QAction* mObjectLayer{};
};

class LayerWidgetContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit LayerWidgetContextMenu(AddLayerContextMenu* menu,
                                  QWidget* parent = nullptr);

 private:
  QMenu* mAdd{};
};

}  // namespace tactile
