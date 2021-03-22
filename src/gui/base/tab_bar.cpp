#include "tab_bar.hpp"

#include <QMouseEvent>

namespace tactile::gui {

tab_bar::tab_bar(QWidget* parent) : QTabBar{parent}, m_edit{new QLineEdit{this}}
{
  m_edit->hide();
  connect(m_edit, &QLineEdit::editingFinished, [this] {
    const auto index = m_renameIndex.value();
    setTabText(index, m_edit->text());
    m_edit->hide();
    m_renameIndex.reset();
    emit edited_tab(index);
  });
}

void tab_bar::edit_tab(const int index)
{
  m_renameIndex = index;

  const auto rect = tabRect(index);
  const auto topMargin = 3;
  const auto leftMargin = 6;
  m_edit->show();
  m_edit->move(rect.left() + leftMargin, rect.top() + topMargin);
  m_edit->resize(rect.width() - 2 * leftMargin, rect.height() - 2 * topMargin);
  m_edit->setText(tabText(index));
  m_edit->selectAll();
  m_edit->setFocus();
}

void tab_bar::mouseDoubleClickEvent(QMouseEvent* event)
{
  QWidget::mouseDoubleClickEvent(event);
  if (const auto index = tabAt(event->pos()); index != -1)
  {
    edit_tab(index);
    event->accept();
  }
}

}  // namespace tactile::gui
