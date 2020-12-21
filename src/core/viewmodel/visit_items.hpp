#pragma once

#include <QStandardItem>  // QStandardItem
#include <concepts>       // invocable

namespace tactile::viewmodel {

template <std::invocable<QStandardItem*> T>
void visit_items(QStandardItemModel* model, const int column, T&& callable)
{
  const auto topLevelRows = model->rowCount();
  const auto* root = model->invisibleRootItem();

  for (auto row = 0; row < topLevelRows; ++row) {
    const auto* topLevelItem = root->child(row, 0);

    for (auto subRow = 0; subRow < topLevelItem->rowCount(); ++subRow) {
      auto* item =
          dynamic_cast<QStandardItem*>(topLevelItem->child(subRow, column));
      if (item) {
        callable(item);
      }
    }
  }
}

}  // namespace tactile::viewmodel
