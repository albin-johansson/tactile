#pragma once

#include <QWidget>

#include "layer_model.hpp"
#include "map_document.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

namespace Ui {
class layer_widget;
}

namespace tactile::gui {

class layer_list_view;
class layer_item;
class add_layer_context_menu;
class layer_item_context_menu;
class layer_widget_context_menu;

class layer_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit layer_widget(QWidget* visible = nullptr);

  ~layer_widget() noexcept override;

 public slots:
  void selected_map(not_null<core::map_document*> document);

 private:
  Ui::layer_widget* m_ui{};
  layer_list_view* m_listView{};
  add_layer_context_menu* m_addLayerMenu{};
  layer_widget_context_menu* m_widgetMenu{};
  layer_item_context_menu* m_itemMenu{};
  unique<vm::layer_model> m_model;

  void update_actions(const maybe<QModelIndex>& selected);

 private slots:
  [[maybe_unused]] void spawn_context_menu(const QPoint& pos);

  [[maybe_unused]] void when_selection_changed(
      const maybe<QModelIndex>& selected,
      const maybe<QModelIndex>& deselected);

  [[maybe_unused]] void new_tile_layer_requested();

  [[maybe_unused]] void new_object_layer_requested();

  [[maybe_unused]] void on_newLayerButton_pressed();

  [[maybe_unused]] void on_removeLayerButton_pressed();

  [[maybe_unused]] void on_upButton_pressed();

  [[maybe_unused]] void on_downButton_pressed();

  [[maybe_unused]] void on_duplicateButton_pressed();

  [[maybe_unused]] void on_visibleButton_toggled(bool visible);

  [[maybe_unused]] void on_opacitySlider_valueChanged(int value);
};

}  // namespace tactile::gui
