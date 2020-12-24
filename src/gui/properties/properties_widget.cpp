#include "properties_widget.hpp"

#ifdef QT_DEBUG
#include <QDebug>
#endif  // QT_DEBUG

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
  m_ui->removeButton->setEnabled(false);
  m_ui->renameButton->setEnabled(false);
  m_contextMenu->disable_all();
  m_contextMenu->set_add_enabled(true);

  // clang-format off
  connect(m_ui->addButton, &QPushButton::pressed,
          this, &properties_widget::new_property_requested);

  connect(m_ui->removeButton, &QPushButton::pressed,
          this, &properties_widget::remove_property_requested);

  connect(m_ui->renameButton, &QPushButton::pressed,
          this, &properties_widget::rename_property_requested);

  connect(m_treeView, &property_tree_view::spawn_context_menu,
          this, &properties_widget::spawn_context_menu);

  connect(m_treeView, &property_tree_view::selection_changed,
          this, &properties_widget::selection_changed);

  connect(m_treeView, &QTreeView::doubleClicked,
          this, &properties_widget::when_double_clicked);

  connect(m_contextMenu, &property_context_menu::copy,
          this, &properties_widget::copy_property_requested);

  connect(m_contextMenu, &property_context_menu::paste,
          this, &properties_widget::paste_property_requested);

  connect(m_contextMenu, &property_context_menu::add,
          this, &properties_widget::new_property_requested);

  connect(m_contextMenu, &property_context_menu::remove,
          this, &properties_widget::remove_property_requested);

  connect(m_contextMenu, &property_context_menu::rename,
          this, &properties_widget::rename_property_requested);

  connect(m_contextMenu, &property_context_menu::change_type,
          this, &properties_widget::change_type_requested);
  // clang-format on
}

properties_widget::~properties_widget() noexcept
{
  delete m_ui;
}

void properties_widget::selected_map(
    const core::map_document& document,
    const vm::shared_property_model& propertyModel)
{
  if (m_model) {
    m_model->disconnect(m_treeView);
    m_model->clear_predefined();
  }

  Q_ASSERT(!propertyModel->parent());

  auto* selectionModel = m_treeView->selectionModel();
  m_treeView->setModel(propertyModel.get());
  m_model = propertyModel;
  selectionModel->deleteLater();

  Q_ASSERT(!m_model->parent());

  m_treeView->add_item_widgets();
  m_treeView->expandAll();

  // clang-format off
  connect(m_model.get(), &vm::property_model::added_file,
          m_treeView,    &property_tree_view::when_file_added);

  connect(m_model.get(), &vm::property_model::added_color,
          m_treeView,    &property_tree_view::when_color_added);

  connect(m_model.get(), &vm::property_model::changed_type,
          m_treeView,    &property_tree_view::when_changed_type);
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

  m_contextMenu->disable_all();
  m_contextMenu->set_add_enabled(true);
}

void properties_widget::selection_changed(const maybe<QModelIndex> index)
{
  m_contextMenu->disable_all();
  m_ui->addButton->setEnabled(true);

  m_contextMenu->set_add_enabled(m_ui->addButton->isEnabled());

  if (!index) {
    return;
  }

  if (const auto* item = m_model->itemFromIndex(*index); !item) {
    return;
  }

  const auto isCustom = m_model->is_custom_property(*index);
  m_ui->removeButton->setEnabled(isCustom);
  m_ui->renameButton->setEnabled(isCustom);

  m_contextMenu->set_remove_enabled(m_ui->removeButton->isEnabled());
  m_contextMenu->set_rename_enabled(m_ui->renameButton->isEnabled());
  m_contextMenu->set_copy_enabled(isCustom);
  m_contextMenu->set_change_type_enabled(isCustom);
  if (isCustom) {
    const auto& property = m_model->get_property(property_name(*index));
    m_contextMenu->set_current_type(property.get_type().value());
  }
}

void properties_widget::copy_property_requested()
{
  const auto currentName = current_property_name();
  m_nameCopy = currentName;
  m_propertyCopy = m_model->get_property(currentName);
}

void properties_widget::paste_property_requested()
{
  if (!m_model->contains_property(m_nameCopy.value())) {
    m_model->add(*m_nameCopy, m_propertyCopy.value());
  }
}

void properties_widget::new_property_requested()
{
  add_property_dialog::spawn(
      [this](const QString& name, const core::property::type type) {
        m_model->add(name, type);
      },
      m_model.get(),
      this);
}

void properties_widget::remove_property_requested()
{
#ifdef QT_DEBUG
  const auto index = m_treeView->currentIndex();
  Q_ASSERT(m_model->is_custom_property(index));
#endif  // QT_DEBUG

  m_model->remove(current_property_name());
}

void properties_widget::rename_property_requested()
{
  const auto oldName = current_property_name();
  if (const auto newName = change_property_name_dialog::spawn(m_model.get())) {
    m_model->rename(oldName, *newName);
  }
}

void properties_widget::change_type_requested(const core::property::type type)
{
  m_model->change_type(current_property_name(), type);
}

void properties_widget::when_double_clicked()
{
  const auto index = m_treeView->currentIndex();
  const auto* item = m_model->itemFromIndex(index);
  if (index.column() == 0 && index.parent().isValid() && item->isEnabled()) {
    const auto oldName = item->text();
    if (const auto newName =
            change_property_name_dialog::spawn(m_model.get())) {
      m_model->rename(oldName, *newName);
    }
  }
}

void properties_widget::spawn_context_menu(const QPoint& pos)
{
  m_contextMenu->set_paste_enabled(m_nameCopy && m_propertyCopy &&
                                   !m_model->contains_property(*m_nameCopy));
  m_contextMenu->exec(pos);
}

auto properties_widget::property_name(const QModelIndex& index) const -> QString
{
  const auto* item = m_model->itemFromIndex(index);
  Q_ASSERT(item);

  if (item->column() == 0) {
    return item->text();
  } else {
    return m_model->itemFromIndex(index.siblingAtColumn(0))->text();
  }
}

auto properties_widget::current_property_name() const -> QString
{
  const auto index = m_treeView->currentIndex();
  Q_ASSERT(index.isValid());
  return property_name(index);
}

}  // namespace tactile::gui
