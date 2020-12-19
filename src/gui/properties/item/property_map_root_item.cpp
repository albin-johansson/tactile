#include "property_map_root_item.hpp"

#include "icons.hpp"
#include "preferences.hpp"
#include "property_int_item.hpp"
#include "property_tree_item.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

property_map_root_item::property_map_root_item(QTreeWidget* parent)
    : QTreeWidgetItem{parent}
{
  setFirstColumnSpanned(true);
  setIcon(0, icons::expanded());
  setText(0, TACTILE_QSTRING(u"Map"));
  setExpanded(true);

  // clang-format off
  m_rows = new property_int_item{TACTILE_QSTRING(u"Rows"), core::property{0}, this};
  m_cols = new property_int_item{TACTILE_QSTRING(u"Columns"), core::property{0}, this};
  m_tileWidth = new property_int_item{TACTILE_QSTRING(u"Tile width"),
                                      core::property{prefs::saves::tile_width().value()},
                                      this};
  m_tileHeight = new property_int_item{TACTILE_QSTRING(u"Tile height"),
                                       core::property{prefs::saves::tile_height().value()},
                                       this};
  // clang-format on

  m_rows->set_name_editable(false);
  m_rows->set_value_editable(false);
  m_rows->setDisabled(true);

  m_cols->set_name_editable(false);
  m_cols->set_value_editable(false);
  m_cols->setDisabled(true);

  m_tileWidth->set_name_editable(false);
  m_tileWidth->set_value_editable(true);

  m_tileHeight->set_name_editable(false);
  m_tileHeight->set_value_editable(true);
}

void property_map_root_item::update(const core::map_document& document)
{
  m_rows->setText(1, QString::number(document.row_count().get()));
  m_cols->setText(1, QString::number(document.col_count().get()));
}

}  // namespace tactile::gui
