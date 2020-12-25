#pragma once

#include <QMenu>

namespace tactile::gui {

class layer_widget_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit layer_widget_context_menu(QWidget* parent = nullptr);

 signals:
  void add_tile_layer();
  void add_object_layer();

 private:
  QMenu* m_add{};

  QAction* m_tileLayer{};
  QAction* m_objectLayer{};
};

}  // namespace tactile::gui
