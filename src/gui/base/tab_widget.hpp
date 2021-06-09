#pragma once

#include <QTabWidget>  // QTabWidget

namespace tactile {

class TabWidget : public QTabWidget
{
  Q_OBJECT

 public:
  explicit TabWidget(QWidget* parent = nullptr);

  void ApplyStylesheet();

  void EditTab(int index);

 signals:
  void S_EditedTab(int index);
};

}  // namespace tactile
