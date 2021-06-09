#include "properties_widget.hpp"

#include <QMouseEvent>  // QMouseEvent

#include "add_property_dialog.hpp"
#include "change_property_name_dialog.hpp"
#include "init_ui.hpp"
#include "map_document.hpp"
#include "preferences.hpp"
#include "property_context_menu.hpp"
#include "property_model.hpp"
#include "property_tree_view.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::properties_widget>(this)}
    , m_contextMenu{new property_context_menu{this}}
    , m_view{new property_tree_view{this}}
{
  m_ui->gridLayout->addWidget(m_view);
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

  connect(m_view, &property_tree_view::spawn_context_menu,
          this, &properties_widget::spawn_context_menu);

  connect(m_view, &property_tree_view::selection_changed,
          this, &properties_widget::selection_changed);

  connect(m_view, &QTreeView::doubleClicked,
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

properties_widget::~properties_widget() noexcept = default;

void properties_widget::show_map(not_null<core::property_manager*> manager)
{
  change_model(manager);
  m_model->set_root_name(tr("Map"));
}

void properties_widget::show_layer(not_null<core::property_manager*> manager)
{
  change_model(manager);
  m_model->set_root_name(tr("Layer"));
}

void properties_widget::added_property(const QString& name)
{
  m_model->added_property(name);
}

void properties_widget::about_to_remove_property(const QString& name)
{
  m_model->about_to_remove_property(name);
}

void properties_widget::updated_property(const QString& name)
{
  m_model->updated_property(name);
}

void properties_widget::renamed_property(const QString& oldName,
                                         const QString& newName)
{
  m_model->renamed_property(oldName, newName);
}

void properties_widget::changed_property_type(const QString& name)
{
  m_model->changed_property_type(name);
}

void properties_widget::selection_changed(const maybe<QModelIndex> index)
{
  m_contextMenu->disable_all();

  m_contextMenu->set_add_enabled(true);
  m_contextMenu->set_paste_enabled(m_nameCopy.has_value());

  m_ui->addButton->setEnabled(true);
  m_ui->removeButton->setEnabled(false);
  m_ui->renameButton->setEnabled(false);

  if (!index || !m_model->itemFromIndex(*index))
  {
    return;
  }

  const auto isCustom = m_model->is_custom_property(*index);
  m_ui->removeButton->setEnabled(isCustom);
  m_ui->renameButton->setEnabled(isCustom);

  m_contextMenu->set_copy_enabled(isCustom);
  m_contextMenu->set_change_type_enabled(isCustom);
  m_contextMenu->set_remove_enabled(m_ui->removeButton->isEnabled());
  m_contextMenu->set_rename_enabled(m_ui->renameButton->isEnabled());
  if (isCustom)
  {
    const auto& property = m_model->get_property(property_name(*index));
    m_contextMenu->set_current_type(property.type().value());
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
  const auto& name = m_nameCopy.value();
  if (!m_model->contains_property(name))
  {
    m_model->add(name, m_propertyCopy.value());
    m_contextMenu->set_paste_enabled(false);
  }
}

void properties_widget::new_property_requested()
{
  AddPropertyDialog::Spawn(
      [this](const QString& name, const core::property_type type) {
        m_model->add(name, type);
      },
      m_model.get(),
      this);
}

void properties_widget::remove_property_requested()
{
#ifdef QT_DEBUG
  const auto index = m_view->currentIndex();
  Q_ASSERT(m_model->is_custom_property(index));
#endif  // QT_DEBUG

  m_model->remove(current_property_name());
}

void properties_widget::rename_property_requested()
{
  const auto oldName = current_property_name();
  if (const auto newName = ChangePropertyNameDialog::Spawn(m_model.get()))
  {
    m_model->rename(oldName, *newName);
  }
}

void properties_widget::change_type_requested(const core::property_type type)
{
  m_model->change_type(current_property_name(), type);
}

void properties_widget::when_double_clicked()
{
  const auto index = m_view->currentIndex();
  const auto* item = m_model->itemFromIndex(index);
  if (index.column() == 0 && index.parent().isValid() && item->isEnabled())
  {
    const auto oldName = item->text();
    if (const auto name = ChangePropertyNameDialog::Spawn(m_model.get()))
    {
      m_model->rename(oldName, *name);
    }
  }
}

void properties_widget::spawn_context_menu(const QPoint& pos)
{
  m_contextMenu->exec(pos);
}

void properties_widget::change_model(not_null<core::property_manager*> manager)
{
  Q_ASSERT(manager);

  m_view->collapseAll();

  m_model = std::make_unique<vm::property_model>(manager);
  Q_ASSERT(!m_model->parent());

  m_view->setModel(m_model.get());
  Q_ASSERT(!m_model->parent());

  // clang-format off
  connect(m_model.get(), &vm::property_model::changed_type,
          m_view,        &property_tree_view::when_changed_type);

  connect(m_model.get(), &vm::property_model::added_file,
          m_view,        &property_tree_view::when_file_added);

  connect(m_model.get(), &vm::property_model::added_color,
          m_view,        &property_tree_view::when_color_added);

  connect(m_model.get(), &vm::property_model::updated_file,
          m_view,        &property_tree_view::when_file_updated);

  connect(m_model.get(), &vm::property_model::updated_color,
          m_view,        &property_tree_view::when_color_updated);
  // clang-format on

  m_view->setFirstColumnSpanned(0, m_view->rootIndex(), true);
  m_view->restore_item_widgets();

  m_contextMenu->disable_all();
  m_contextMenu->set_add_enabled(true);

  m_view->expandAll();
}

auto properties_widget::property_name(const QModelIndex& index) const -> QString
{
  const auto* item = m_model->itemFromIndex(index);
  Q_ASSERT(item);

  if (item->column() == 0)
  {
    return item->text();
  }
  else
  {
    return m_model->itemFromIndex(index.siblingAtColumn(0))->text();
  }
}

auto properties_widget::current_property_name() const -> QString
{
  const auto index = m_view->currentIndex();
  Q_ASSERT(index.isValid());
  return property_name(index);
}

}  // namespace tactile::gui
