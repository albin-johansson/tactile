#pragma once

#include <QDockWidget>

#include "layer.hpp"
#include "map_document.hpp"

namespace tactile::gui {

class layer_widget;

class layer_dock final : public QDockWidget
{
  Q_OBJECT

 public:
  explicit layer_dock(QWidget* parent = nullptr);

 signals:
  void ui_add_layer();
  void ui_remove_layer(layer_id id);
  void ui_select_layer(layer_id id);
  void ui_set_layer_visibility(layer_id id, bool visible);
  void ui_set_layer_opacity(layer_id id, double opacity);
  void ui_set_layer_name(layer_id id, const QString& name);
  void ui_move_layer_down(layer_id id);
  void ui_move_layer_up(layer_id id);
  void ui_duplicate_layer(layer_id id);

 public slots:
  void added_layer(layer_id id, const core::layer& layer);

  void added_duplicated_layer(layer_id id, const core::layer& layer);

  void removed_layer(layer_id id);

  void selected_layer(layer_id id, const core::layer& layer);

  void selected_map(const core::map_document& document);

  void moved_layer_back(layer_id id);

  void moved_layer_forward(layer_id id);

 private:
  layer_widget* m_widget{};
};

}  // namespace tactile::gui
