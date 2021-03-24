#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, layer_widget)

TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, layer)
TACTILE_FORWARD_DECLARE(tactile::vm, layer_model)
TACTILE_FORWARD_DECLARE(tactile::gui, layer_list_view)
TACTILE_FORWARD_DECLARE(tactile::gui, layer_item)
TACTILE_FORWARD_DECLARE(tactile::gui, add_layer_context_menu)
TACTILE_FORWARD_DECLARE(tactile::gui, layer_item_context_menu)
TACTILE_FORWARD_DECLARE(tactile::gui, layer_widget_context_menu)

namespace tactile::gui {

class layer_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit layer_widget(QWidget* visible = nullptr);

  ~layer_widget() noexcept override;

 public slots:
  void selected_map(not_null<core::map_document*> document);

 private:
  unique<Ui::layer_widget> m_ui;
  layer_list_view* m_view{};
  add_layer_context_menu* m_addLayerMenu{};
  layer_widget_context_menu* m_widgetMenu{};
  layer_item_context_menu* m_itemMenu{};
  unique<vm::layer_model> m_model;

  void update_actions(const maybe<QModelIndex>& selected);

 private slots:
  void changed_layer_opacity(layer_id id, double opacity);

  void selected_layer(layer_id id, const core::layer& layer);

  void spawn_context_menu(const QPoint& pos);

  void when_selection_changed(maybe<QModelIndex> selected,
                              maybe<QModelIndex> deselected);

  void new_tile_layer_requested();

  void new_object_layer_requested();

  [[maybe_unused]] void on_newLayerButton_pressed();

  [[maybe_unused]] void on_removeLayerButton_pressed();

  [[maybe_unused]] void on_upButton_pressed();

  [[maybe_unused]] void on_downButton_pressed();

  [[maybe_unused]] void on_duplicateButton_pressed();

  [[maybe_unused]] void on_visibleButton_toggled(bool visible);

  [[maybe_unused]] void on_opacitySlider_valueChanged(int value);
};

}  // namespace tactile::gui
