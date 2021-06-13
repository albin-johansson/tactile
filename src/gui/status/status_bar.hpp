#pragma once

#include <QComboBox>   // QComboBox
#include <QLabel>      // QLabel
#include <QStatusBar>  // QStatusBar

#include "forward_declare.hpp"
#include "layer_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

class StatusBar final : public QStatusBar
{
  Q_OBJECT

 public:
  explicit StatusBar(QWidget* parent = nullptr);

  void SetLayerComboBoxVisible(bool visible);

  void SetMouseInfoVisible(bool visible);

  void SetCurrentLayer(layer_id id);

 signals:
  void S_SelectLayerRequest(layer_id id);

 public slots:
  void OnSwitchedMap(const core::MapDocument& document);

  void OnAddedLayer(layer_id id, const QString& name);

  void SetLayerName(layer_id id, const QString& name);

  void OnRemovedLayer(layer_id id);

  void OnMouseMoved(const QPointF& pos);

 private:
  QComboBox* mLayerBox{};
  QLabel* mMouseXLabel{};
  QLabel* mMouseYLabel{};
  QLabel* mMouseXValueLabel{};
  QLabel* mMouseYValueLabel{};

 private slots:
  void OnCurrentIndexChanged(int index);
};

}  // namespace tactile
