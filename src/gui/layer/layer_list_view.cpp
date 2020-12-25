#include "layer_list_view.hpp"

#include "tactile_qstring.hpp"

namespace tactile::gui {

layer_list_view::layer_list_view(QWidget* parent) : QListView{parent}
{
  setObjectName(TACTILE_QSTRING(u"layer_list_view"));

  setSelectionBehavior(SelectRows);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(false);
}

void layer_list_view::selectionChanged(const QItemSelection& selected,
                                       const QItemSelection& deselected)
{
  QListView::selectionChanged(selected, deselected);
}

void layer_list_view::mousePressEvent(QMouseEvent* event)
{
  // This is done to be able to deselect layers by pressing outside of items
  selectionModel()->clear();
  QListView::mousePressEvent(event);
}

}  // namespace tactile::gui
