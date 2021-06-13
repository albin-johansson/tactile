#include "tab_bar.hpp"

#include <QMouseEvent>

namespace tactile {

TabBar::TabBar(QWidget* parent) : QTabBar{parent}, mEdit{new QLineEdit{this}}
{
  mEdit->hide();
  connect(mEdit, &QLineEdit::editingFinished, this, &TabBar::OnEditingFinished);
}

void TabBar::EditTab(const int index)
{
  mRenameIndex = index;

  const auto rect = tabRect(index);
  const auto topMargin = 3;
  const auto leftMargin = 6;
  mEdit->show();
  mEdit->move(rect.left() + leftMargin, rect.top() + topMargin);
  mEdit->resize(rect.width() - 2 * leftMargin, rect.height() - 2 * topMargin);
  mEdit->setText(tabText(index));
  mEdit->selectAll();
  mEdit->setFocus();
}

void TabBar::mouseDoubleClickEvent(QMouseEvent* event)
{
  QTabBar::mouseDoubleClickEvent(event);
  if (const auto index = tabAt(event->pos()); index != -1)
  {
    EditTab(index);
    event->accept();
  }
}

void TabBar::OnEditingFinished()
{
  const auto index = mRenameIndex.value();
  setTabText(index, mEdit->text());

  mEdit->hide();
  mRenameIndex.reset();

  emit S_EditedTab(index);
}

}  // namespace tactile
