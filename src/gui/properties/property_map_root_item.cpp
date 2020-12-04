#include "property_map_root_item.hpp"

#include "icons.hpp"
#include "preferences.hpp"
#include "tactile_qstring.hpp"
#include "tree_widget_utils.hpp"

namespace tactile::gui {

property_map_root_item::property_map_root_item(QTreeWidget* parent)
    : QTreeWidgetItem{parent}
{
  setFirstColumnSpanned(true);
  setIcon(0, icons::expanded());
  setText(0, TACTILE_QSTRING(u"Map"));
  setExpanded(true);

  m_rows = new property_tree_item{TACTILE_QSTRING(u"Rows"), 0, this};
  m_cols = new property_tree_item{TACTILE_QSTRING(u"Columns"), 0, this};

  m_tileWidth = new property_tree_item{TACTILE_QSTRING(u"Tile width"),
                                       prefs::saves::tile_width().value(),
                                       this};
  m_tileHeight = new property_tree_item{TACTILE_QSTRING(u"Tile height"),
                                        prefs::saves::tile_height().value(),
                                        this};

  m_rows->setDisabled(true);
  m_cols->setDisabled(true);
}

void property_map_root_item::update(const core::map_document& document)
{
  m_rows->setText(1, QString::number(document.row_count().get()));
  m_cols->setText(1, QString::number(document.col_count().get()));
}

}  // namespace tactile::gui
