#pragma once

#include <QStandardItemModel>
#include <QTreeView>

#include "vector_map.hpp"

namespace tactile::viewmodel {
class property_model;
}

namespace tactile::gui {

class property_tree_view final : public QTreeView
{
  Q_OBJECT

 public:
  explicit property_tree_view(QWidget* parent = nullptr);

  void setModel(QAbstractItemModel* model) override;

  void add_item_widgets();

 public slots:
  void when_color_added(const QModelIndex& index);

  void when_file_added(const QModelIndex& index);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;

  void mousePressEvent(QMouseEvent* event) override;

  auto edit(const QModelIndex& index, EditTrigger trigger, QEvent* event)
      -> bool override;

 private:
  vector_map<int, QStandardItem*> m_widgetItems;
  int m_nextWidgetId{1};

  [[nodiscard]] auto get_model() -> viewmodel::property_model*;

  [[nodiscard]] auto get_model() const -> const viewmodel::property_model*;

  [[nodiscard]] auto new_widget_id() noexcept -> int
  {
    const auto next = m_nextWidgetId;
    ++m_nextWidgetId;
    return next;
  }
};

}  // namespace tactile::gui
