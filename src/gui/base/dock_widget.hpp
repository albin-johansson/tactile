#pragma once

#include <QDockWidget>  // QDockWidget

namespace tactile {

class DockWidget : public QDockWidget
{
  Q_OBJECT

 protected:
  explicit DockWidget(QWidget* parent = nullptr);

  void closeEvent(QCloseEvent* event) override;

 signals:
  void S_Closed();
};

}  // namespace tactile
