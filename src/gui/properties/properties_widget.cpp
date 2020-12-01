#include "properties_widget.hpp"

#include "icons.hpp"
#include "preferences.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {

inline constexpr int nameColumn = 0;
inline constexpr int valueColumn = 1;

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::properties_widget{}}
{
  m_ui->setupUi(this);

  connect(m_ui->treeWidget,
          &QTreeWidget::itemCollapsed,
          [](QTreeWidgetItem* item) {
            item->setIcon(nameColumn, icons::collapsed());
          });

  connect(m_ui->treeWidget,
          &QTreeWidget::itemExpanded,
          [](QTreeWidgetItem* item) {
            item->setIcon(nameColumn, icons::expanded());
          });

}

void properties_widget::selected_map(const core::map_document& document)
{
  m_ui->treeWidget->clear();

  auto* documentItem = new QTreeWidgetItem{m_ui->treeWidget};
  documentItem->setIcon(nameColumn, icons::collapsed());
  documentItem->setText(nameColumn, TACTILE_QSTRING(u"Document"));
  documentItem->setFirstColumnSpanned(true);

  auto* rows = new QTreeWidgetItem{documentItem};
  rows->setText(nameColumn, TACTILE_QSTRING(u"Rows"));
  rows->setText(valueColumn, QString::number(document.row_count().get()));
  rows->setDisabled(true);

  auto* cols = new QTreeWidgetItem{documentItem};
  cols->setText(nameColumn, TACTILE_QSTRING(u"Columns"));
  cols->setText(valueColumn, QString::number(document.col_count().get()));
  cols->setDisabled(true);

  auto* tileWidth = new QTreeWidgetItem{documentItem};
  tileWidth->setText(nameColumn, TACTILE_QSTRING(u"Tile width"));
  tileWidth->setText(valueColumn,
                     QString::number(prefs::saves::tile_width().value()));
  tileWidth->setDisabled(true);

  auto* tileHeight = new QTreeWidgetItem{documentItem};
  tileHeight->setText(nameColumn, TACTILE_QSTRING(u"Tile height"));
  tileHeight->setText(valueColumn,
                      QString::number(prefs::saves::tile_height().value()));
  tileHeight->setDisabled(true);

  auto* custom = new QTreeWidgetItem{m_ui->treeWidget};
  custom->setIcon(nameColumn, icons::collapsed());
  custom->setText(nameColumn, TACTILE_QSTRING(u"Custom"));
  custom->setFirstColumnSpanned(true);
}

void properties_widget::select_layer(const core::layer& layer)
{}

void properties_widget::added_property(const QString& name,
                                       const core::property& property)
{}

void properties_widget::updated_property(const QString& name,
                                         const core::property& property)
{}

void properties_widget::removed_property(const QString& name)
{}

}  // namespace tactile::gui
