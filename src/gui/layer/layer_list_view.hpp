#pragma once

#include <QListView>  // QListView

#include "maybe.hpp"

namespace tactile::gui {

class layer_list_view final : public QListView
{
  Q_OBJECT

 public:
  explicit layer_list_view(QWidget* parent = nullptr);

  void select_quietly(const QModelIndex& index);

 signals:
  void selection_changed(const maybe<QModelIndex>& selected,
                         const maybe<QModelIndex>& deselected);

  void changed_name(const QModelIndex& index, const QString& name);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;

  void dataChanged(const QModelIndex& topLeft,
                   const QModelIndex& bottomRight,
                   const QList<int>& roles) override;

 private:
  bool m_quiet{};
};

}  // namespace tactile::gui
