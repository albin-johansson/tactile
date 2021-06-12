#pragma once

#include <QLineEdit>  // QLineEdit
#include <QTabBar>    // QTabBar

#include "maybe.hpp"

namespace tactile {

class TabBar final : public QTabBar
{
  Q_OBJECT

 public:
  explicit TabBar(QWidget* parent = nullptr);

  void EditTab(int index);

 signals:
  void S_EditedTab(int index);

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  QLineEdit* mEdit{};
  Maybe<int> mRenameIndex;
};

}  // namespace tactile
