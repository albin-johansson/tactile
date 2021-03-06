#pragma once

#include <QMenu>  // QMenu

namespace tactile::gui {

class map_editor_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit map_editor_context_menu(QWidget* parent = nullptr);

 signals:
  void show_properties();

 private:
  QAction* m_showProperties{};
};

}  // namespace tactile::gui
