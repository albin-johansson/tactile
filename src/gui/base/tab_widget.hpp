#pragma once

#include <QTabWidget>  // QTabWidget

namespace tactile::gui {

class tab_widget final : public QTabWidget
{
  Q_OBJECT

 public:
  explicit tab_widget(QWidget* parent = nullptr);

  void edit_tab(int index);

 signals:
  void edited_tab(int index);
};

}  // namespace tactile::gui
