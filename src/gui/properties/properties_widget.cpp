#include "properties_widget.hpp"

#include <QDebug>

#include "add_property_dialog.hpp"
#include "change_property_name_dialog.hpp"
#include "init_ui.hpp"
#include "preferences.hpp"
#include "property_context_menu.hpp"
#include "property_tree_view.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::properties_widget>(this)}
    , m_contextMenu{new property_context_menu{this}}
    , m_treeView{new property_tree_view{this}}
{
  m_ui->gridLayout->addWidget(m_treeView, 1, 1);

  // clang-format off
  connect(m_ui->addButton, &QPushButton::pressed, this, &properties_widget::new_property_requested);
  connect(m_treeView, &QTreeView::doubleClicked, this, &properties_widget::rename_property_requested);
  // clang-format on
}

properties_widget::~properties_widget() noexcept
{
  delete m_ui;
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
  connect(m_model, &viewmodel::property_model::added_file, m_treeView, &property_tree_view::when_file_added);
  connect(m_model, &viewmodel::property_model::added_color, m_treeView, &property_tree_view::when_color_added);
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

void properties_widget::rename_property_requested(const QModelIndex& index)
{
  auto* item = m_model->itemFromIndex(index);
  if (index.column() == 0 && index.parent().isValid() && item->isEnabled()) {
    const auto oldName = item->text();
    if (const auto newName = change_property_name_dialog::spawn(m_model)) {
      m_model->rename(oldName, *newName);
    }
  }
}

}  // namespace tactile::gui
