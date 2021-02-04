#pragma once

#include <QMenu>  // QMenu

namespace tactile::gui {

class add_layer_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit add_layer_context_menu(QWidget* parent = nullptr);

 signals:
  void add_tile_layer();
  void add_object_layer();

 private:
  QAction* m_tileLayer{};
  QAction* m_objectLayer{};
};

class layer_widget_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit layer_widget_context_menu(add_layer_context_menu* addLayerMenu,
                                     QWidget* parent = nullptr);

 private:
  QMenu* m_add{};
};

}  // namespace tactile::gui
