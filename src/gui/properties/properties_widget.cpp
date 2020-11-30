#include "properties_widget.hpp"

#include "icons.hpp"
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
