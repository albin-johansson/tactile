#pragma once

#include <QMenu>

namespace tactile::gui {

class tileset_tab_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit tileset_tab_context_menu(QWidget* parent = nullptr);

  void set_tab_index(int index);

 signals:
  void rename(int index);
  void remove(int index);

 private:
  QAction* m_rename{};
  [[maybe_unused]] QAction* m_sep0{};
  QAction* m_remove{};
  int m_index{-1};
};

}  // namespace tactile::gui
