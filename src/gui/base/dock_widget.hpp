#pragma once

#include <QDockWidget>  // QDockWidget

namespace tactile::gui {

class dock_widget : public QDockWidget
{
  Q_OBJECT

 protected:
  explicit dock_widget(QWidget* parent = nullptr);

  void closeEvent(QCloseEvent* event) override;

 signals:
  void closed();
};

}  // namespace tactile::gui
