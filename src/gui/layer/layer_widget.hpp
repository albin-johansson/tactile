#pragma once

#include <QListWidgetItem>
#include <QWidget>
#include <optional>  // optional

#include "layer.hpp"
#include "map_document.hpp"

namespace Ui {
class layer_widget;
}

namespace tactile::gui {

class layer_item;

class layer_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit layer_widget(QWidget* parent = nullptr);

  ~layer_widget() noexcept override;

 signals:
  void ui_requested_new_layer();

  void ui_requested_remove_layer();

  void ui_selected_layer(layer_id id);

  void ui_set_layer_opacity(double opacity);

  void ui_set_layer_visibility(bool visible);

  void ui_set_layer_name(const QString& name);

 public slots:
  void added_layer(layer_id id, const core::layer& layer);

  void removed_layer(layer_id id);

  void selected_layer(layer_id id, const core::layer& layer);

  void selected_map(const core::map_document& document);

 private:
  Ui::layer_widget* m_ui{};
  int m_nameSuffix{1};

  void trigger_layer_item_context_menu(const QPoint& pos);

  void add_layer(layer_id id);

  [[nodiscard]] auto item_for_layer_id(layer_id id) -> layer_item*;

  void update_possible_actions();

 private slots:
  void current_item_changed(QListWidgetItem* current,
                            QListWidgetItem* previous);

  void item_changed(QListWidgetItem* item);
};

}  // namespace tactile::gui
