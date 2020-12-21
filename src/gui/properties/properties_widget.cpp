#include "properties_widget.hpp"

#include <QDebug>

#include "add_property_dialog.hpp"
#include "file_value_widget.hpp"
#include "preferences.hpp"
#include "properties_context_menu.hpp"
#include "property_tree_view.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::properties_widget{}}
{
  m_ui->setupUi(this);
  m_treeView = new property_tree_view{this};
  m_ui->gridLayout->addWidget(m_treeView, 1, 1);

  // clang-format off
  connect(m_ui->addButton, &QPushButton::pressed, this, &properties_widget::new_property_requested);
  // clang-format on
}

void properties_widget::selected_map(const core::map_document& document)
{
  if (m_model) {
    m_model->disconnect(m_treeView);
    m_model->clear_predefined();
  }

  m_model = document.property_viewmodel();

  m_treeView->setModel(m_model);
  m_treeView->add_item_widgets();
  m_treeView->expandAll();

  // clang-format off
  connect(m_model, &viewmodel::property_viewmodel::added_file, m_treeView, &property_tree_view::when_file_added);
  connect(m_model, &viewmodel::property_viewmodel::added_color, m_treeView, &property_tree_view::when_color_added);
  // clang-format on

  m_model->set_predefined_name(TACTILE_QSTRING(u"Map"));
  m_treeView->setFirstColumnSpanned(0, m_treeView->rootIndex(), true);
  m_treeView->setFirstColumnSpanned(1, m_treeView->rootIndex(), true);

  m_model->add_predefined(TACTILE_QSTRING(u"Width"),
                          document.col_count().get());
  m_model->add_predefined(TACTILE_QSTRING(u"Height"),
                          document.row_count().get());
  m_model->add_predefined(TACTILE_QSTRING(u"Tile width"),
                          prefs::saves::tile_width().value(),
                          false);
  m_model->add_predefined(TACTILE_QSTRING(u"Tile height"),
                          prefs::saves::tile_height().value(),
                          false);
}

void properties_widget::new_property_requested()
{
  add_property_dialog::spawn(
      [&](const QString& name, const core::property::type type) {
        m_model->add(name, type);
      },
      m_model,
      this);
}

}  // namespace tactile::gui
