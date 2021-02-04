#pragma once

#include <QLineEdit>  // QLineEdit
#include <QTabBar>    // QTabBar

#include "maybe.hpp"

namespace tactile::gui {

class tab_bar final : public QTabBar
{
  Q_OBJECT

 public:
  explicit tab_bar(QWidget* parent = nullptr);

  void edit_tab(int index);

 signals:
  void edited_tab(int index);

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  QLineEdit* m_edit{};
  maybe<int> m_renameIndex;
};

}  // namespace tactile::gui
