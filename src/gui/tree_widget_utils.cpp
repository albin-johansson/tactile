#include "tree_widget_utils.hpp"

#include "icons.hpp"

namespace tactile::gui {

void setup_expand_collapse_icons(QTreeWidget* widget)
{
  QObject::connect(widget,
                   &QTreeWidget::itemCollapsed,
                   [](QTreeWidgetItem* item) {
                     if (!item->parent()) {
                       item->setIcon(0, icons::collapsed());
                     }
                   });

  QObject::connect(widget,
                   &QTreeWidget::itemExpanded,
                   [](QTreeWidgetItem* item) {
                     if (!item->parent()) {
                       item->setIcon(0, icons::expanded());
                     }
                   });
}

}  // namespace tactile::gui
