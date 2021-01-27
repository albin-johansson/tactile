#pragma once

#include <QTreeView>  // QTreeView

#include "maybe.hpp"
#include "property.hpp"
#include "vector_map.hpp"

class QStandardItem;

namespace tactile::vm {
class property_model;
}

namespace tactile::gui {

class property_tree_view final : public QTreeView
{
  Q_OBJECT

 public:
  explicit property_tree_view(QWidget* parent = nullptr);

  void restore_item_widgets();

 signals:
  void spawn_context_menu(const QPoint& pos);

  void selection_changed(maybe<QModelIndex> index);

 public slots:
  void when_color_added(const QModelIndex& valueIndex);

  void when_file_added(const QModelIndex& valueIndex);

  void when_changed_type(const QModelIndex& valueIndex,
                         core::property::type type);

  void when_file_updated(const QModelIndex& index);

  void when_color_updated(const QModelIndex& index);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;

  void rowsAboutToBeRemoved(const QModelIndex& parent,
                            int start,
                            int end) override;

  void mousePressEvent(QMouseEvent* event) override;

 private:
  vector_map<int, QStandardItem*> m_widgetItems;
  int m_nextWidgetId{1};

  [[nodiscard]] auto get_model() -> vm::property_model*;

  [[nodiscard]] auto get_model() const -> const vm::property_model*;

  [[nodiscard]] auto new_widget_id() noexcept -> int;

 private slots:
  void when_item_expanded(const QModelIndex& index);

  void when_item_collapsed(const QModelIndex& index);
};

}  // namespace tactile::gui
