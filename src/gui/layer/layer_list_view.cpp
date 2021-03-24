#include "layer_list_view.hpp"

#include "tactile_qstring.hpp"

namespace tactile::gui {

layer_list_view::layer_list_view(QWidget* parent) : QListView{parent}
{
  setObjectName(TACTILE_QSTRING(u"layer_list_view"));

  setContextMenuPolicy(Qt::CustomContextMenu);
  setSelectionBehavior(SelectRows);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(false);
}

void layer_list_view::select_quietly(const QModelIndex& index)
{
  m_quiet = true;

  auto* selection = selectionModel();
  selection->clearSelection();
  selection->setCurrentIndex(index, QItemSelectionModel::Select);

  m_quiet = false;
}

void layer_list_view::selectionChanged(const QItemSelection& selected,
                                       const QItemSelection& deselected)
{
  QListView::selectionChanged(selected, deselected);

  Q_ASSERT(selected.empty() || selected.size() == 1);
  Q_ASSERT(deselected.empty() || deselected.size() == 1);

  maybe<QModelIndex> selectedIndex;
  for (const auto index : selected.indexes())
  {
    selectedIndex = index;
  }

  maybe<QModelIndex> deselectedIndex;
  for (const auto index : deselected.indexes())
  {
    deselectedIndex = index;
  }

  if (!m_quiet)
  {
    emit selection_changed(selectedIndex, deselectedIndex);
  }
}

}  // namespace tactile::gui
