#pragma once

#include <QListView>  // QListView

#include "maybe.hpp"

namespace tactile {

class LayerListView final : public QListView
{
  Q_OBJECT

 public:
  explicit LayerListView(QWidget* parent = nullptr);

  void SelectQuietly(const QModelIndex& index);  // TODO remove?

 signals:
  void S_SelectionChanged(const maybe<QModelIndex>& selected,
                          const maybe<QModelIndex>& deselected);

  void S_ChangedName(const QModelIndex& index, const QString& name);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;

  void dataChanged(const QModelIndex& topLeft,
                   const QModelIndex& bottomRight,
                   const QList<int>& roles) override;

 private:
  bool mQuiet{};
};

}  // namespace tactile
