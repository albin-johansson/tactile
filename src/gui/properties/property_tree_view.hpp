#pragma once

#include <QTreeView>  // QTreeView

#include "fwd.hpp"
#include "maybe.hpp"
#include "property.hpp"
#include "tactile_declare_ui.hpp"
#include "vector_map.hpp"

class QStandardItem;

namespace tactile {

class PropertyTreeView final : public QTreeView
{
  Q_OBJECT

 public:
  explicit PropertyTreeView(QWidget* parent = nullptr);

  void RestoreItemWidgets();

 signals:
  void S_SpawnContextMenu(const QPoint& pos);
  void S_SelectionChanged(Maybe<QModelIndex> index);

 public slots:
  void OnColorAdded(const QModelIndex& valueIndex);
  void OnFileAdded(const QModelIndex& valueIndex);
  void OnChangedType(const QModelIndex& valueIndex, core::PropertyType type);
  void OnFileUpdated(const QModelIndex& index);
  void OnColorUpdated(const QModelIndex& index);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;

  void rowsAboutToBeRemoved(const QModelIndex& parent,
                            int start,
                            int end) override;

  void mousePressEvent(QMouseEvent* event) override;

 private:
  vector_map<int, QStandardItem*> mWidgetItems;
  int mNextWidgetId{1};

  [[nodiscard]] auto GetModel() -> vm::PropertyModel*;
  [[nodiscard]] auto GetModel() const -> const vm::PropertyModel*;

  [[nodiscard]] auto NewWidgetId() noexcept -> int;

 private slots:
  void OnItemExpanded(const QModelIndex& index);
  void OnItemCollapsed(const QModelIndex& index);
};

}  // namespace tactile
