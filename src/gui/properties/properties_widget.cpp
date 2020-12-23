#include "properties_widget.hpp"

#include <QDebug>
#include <QMouseEvent>

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
  m_ui->gridLayout->addWidget(m_treeView);

  // clang-format off
  connect(m_ui->addButton, &QPushButton::pressed,
          this, &properties_widget::new_property_requested);

  connect(m_ui->removeButton, &QPushButton::pressed,
          this, &properties_widget::remove_property_requested);

  connect(m_treeView, &property_tree_view::spawn_context_menu,
          this, &properties_widget::spawn_context_menu);

  connect(m_treeView, &property_tree_view::selection_changed,
          this, &properties_widget::selection_changed);

  connect(m_treeView, &QTreeView::doubleClicked,
          this, &properties_widget::rename_property_requested);

  connect(m_contextMenu, &property_context_menu::add,
          this, &properties_widget::new_property_requested);

  connect(m_contextMenu, &property_context_menu::remove,
          this, &properties_widget::remove_property_requested);

  connect(m_contextMenu, &property_context_menu::rename,
          this, &properties_widget::rename_property_requested);
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
  connect(m_model, &viewmodel::property_model::added_file,
          m_treeView, &property_tree_view::when_file_added);

  connect(m_model, &viewmodel::property_model::added_color,
          m_treeView, &property_tree_view::when_color_added);
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

void properties_widget::selection_changed(const QModelIndex& index)
{
  if (const auto* item = m_model->itemFromIndex(index); !item) {
    m_ui->removeButton->setEnabled(false);
    m_ui->renameButton->setEnabled(false);
    return;
  }

  m_ui->addButton->setEnabled(true);

  const auto isCustom = m_model->is_custom_property(index);
  m_ui->removeButton->setEnabled(isCustom);
  m_ui->renameButton->setEnabled(isCustom && index.column() == 0);
}

void properties_widget::new_property_requested()
{
  add_property_dialog::spawn(
      [this](const QString& name, const core::property::type type) {
        m_model->add(name, type);
      },
      m_model,
      this);
}

void properties_widget::remove_property_requested()
{
  const auto index = m_treeView->currentIndex();
  Q_ASSERT(m_model->is_custom_property(index));

  const auto* item = m_model->itemFromIndex(index);
  if (item->column() == 0) {
    m_model->remove(item->text());

  } else {
    const auto sibling = m_model->sibling(item->row(), 0, index);
    m_model->remove(m_model->itemFromIndex(sibling)->text());
  }
}

void properties_widget::rename_property_requested()
{
  const auto index = m_treeView->currentIndex();
  const auto* item = m_model->itemFromIndex(index);
  if (index.column() == 0 && index.parent().isValid() && item->isEnabled()) {
    const auto oldName = item->text();
    if (const auto newName = change_property_name_dialog::spawn(m_model)) {
      m_model->rename(oldName, *newName);
    }
  }
}

void properties_widget::spawn_context_menu(const QPoint& pos)
{
  m_contextMenu->disable_all();

  m_contextMenu->set_add_enabled(m_ui->addButton->isEnabled());
  m_contextMenu->set_remove_enabled(m_ui->removeButton->isEnabled());
  m_contextMenu->set_rename_enabled(m_ui->renameButton->isEnabled());
  //  m_contextMenu->set_duplicate_enabled(m_ui->duplicateButton->isEnabled());

  m_contextMenu->exec(pos);
}

}  // namespace tactile::gui
