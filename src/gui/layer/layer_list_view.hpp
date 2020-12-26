#pragma once

#include <QListView>

#include "maybe.hpp"

namespace tactile::gui {

class layer_list_view final : public QListView
{
  Q_OBJECT

 public:
  explicit layer_list_view(QWidget* parent = nullptr);

 signals:
  void selection_changed(const maybe<QModelIndex>& selected,
                         const maybe<QModelIndex>& deselected);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;
};

}  // namespace tactile::gui
