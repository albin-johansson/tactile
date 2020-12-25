#pragma once

#include <QListWidgetItem>
#include <QWidget>

#include "layer.hpp"
#include "layer_model.hpp"
#include "map_document.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "smart_pointers.hpp"
#include "vector_map.hpp"

namespace Ui {
class layer_widget;
}

namespace tactile::gui {

class layer_list_view;
class layer_item;
class layer_item_context_menu;
class layer_widget_context_menu;

class layer_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit layer_widget(QWidget* visible = nullptr);

  ~layer_widget() noexcept override;

 public slots:
  void selected_map(map_id id,
                    const core::map_document& document,
                    const shared<vm::layer_model>& model);

 private:
  Ui::layer_widget* m_ui{};
  layer_list_view* m_listView{};

  layer_item_context_menu* m_itemContextMenu{};
  layer_widget_context_menu* m_widgetContextMenu{};

  shared<vm::layer_model> m_model;

  vector_map<map_id, int> m_suffixes;
  maybe<map_id> m_currentMap;
  maybe<int> m_duplicateTargetRow;
  maybe<QString> m_cachedName;

 private slots:
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
