#include "property_model.hpp"

#include "file_value_widget.hpp"
#include "item_model_utils.hpp"
#include "make_property_item.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

property_model::property_model(core::property_manager* manager, QObject* parent)
    : QStandardItemModel{parent}
    , m_manager{manager}
    , m_customRoot{new root_item{tr("Properties")}}
{
  if (!m_manager) {
    throw tactile_error{"property_model requires non-null property manager!"};
  }

  setColumnCount(2);
  appendRow(m_customRoot);

  // clang-format off
  connect(this, &property_model::dataChanged,
          this, &property_model::when_data_changed);
  // clang-format on

  for (const auto& [name, property] : manager->properties()) {
    const auto index = add_property_to_gui(name, property, m_customRoot);
    itemFromIndex(index.siblingAtColumn(0))->setEditable(false);
  }
}

void property_model::add(const QString& name, const core::property::type type)
{
  m_manager->add_property(name, type);
  add_existing_property_to_gui(name);
}

auto property_model::add(const QString& name, const core::property& property)
    -> QModelIndex
{
  m_manager->add_property(name, property);
  return add_existing_property_to_gui(name);
}

void property_model::change_type(const QString& name,
                                 const core::property::type type)
{
  if (auto* item = find_item(this, name, 0)) {
    const auto row = item->row();
    removeRow(row, m_customRoot->index());

    m_manager->change_property_type(name, type);
    const auto& property = m_manager->get_property(name);

    auto* valueItem = make_property_item(m_manager->get_property(name));
    m_customRoot->insertRow(row, {new QStandardItem{name}, valueItem});

    emit changed_type(indexFromItem(valueItem), type);
  }
}

void property_model::rename(const QString& oldName, const QString& newName)
{
  rename_property_in_gui(oldName, newName);
  m_manager->rename_property(oldName, newName);
}

void property_model::remove(const QString& name)
{
  remove_property_from_gui(name);
  m_manager->remove_property(name);
}

auto property_model::contains_property(const QString& name) const -> bool
{
  return find_item(this, name, 0) != nullptr;
}

auto property_model::is_custom_property(const QModelIndex& index) const -> bool
{
  return parent(index) == indexFromItem(m_customRoot);
}

auto property_model::get_property(const QString& name) const
    -> const core::property&
{
  return m_manager->get_property(name);
}

void property_model::added_property(const QString& name)
{
  add_existing_property_to_gui(name);
}

void property_model::about_to_remove_property(const QString& name)
{
  remove_property_from_gui(name);
}

void property_model::updated_property(const QString& name)
{
  // TODO fix file and color properties being updated

  m_blockDataChanged = true;

  if (auto* item = find_item(this, name, 0)) {
    const auto& property = m_manager->get_property(name);

    const auto sibling = item->index().siblingAtColumn(1);
    Q_ASSERT(sibling.isValid());

    update_item_data(itemFromIndex(sibling), property);

    // We need to notify the view index widget to update its contents
    const auto type = property.get_type().value();
    if (type == core::property::file) {
      emit updated_file(sibling);

    } else if (type == core::property::color) {
      emit updated_color(sibling);
    }
  }

  m_blockDataChanged = false;
}

void property_model::changed_property_type(const QString& name)
{
  remove_property_from_gui(name);
  add_existing_property_to_gui(name);
}

void property_model::renamed_property(const QString& oldName,
                                      const QString& newName)
{
  rename_property_in_gui(oldName, newName);
}

void property_model::remove_property_from_gui(const QString& name)
{
  if (auto* item = find_item(this, name, 0)) {
    const auto row = item->row();
    removeRow(row, item->parent()->index());
  }
}

auto property_model::add_existing_property_to_gui(const QString& name)
    -> QModelIndex
{
  const auto& prop = m_manager->get_property(name);
  const auto index = add_property_to_gui(name, prop, m_customRoot);

  auto* sibling = itemFromIndex(index.siblingAtColumn(0));
  sibling->setEditable(false);

  return index;
}

auto property_model::add_property_to_gui(const QString& name,
                                         const core::property& property,
                                         QStandardItem* root) -> QModelIndex
{
  auto* nameItem = new QStandardItem{name};
  auto* valueItem = make_property_item(property);

  root->appendRow({nameItem, valueItem});

  const auto index = indexFromItem(valueItem);
  const auto type = property.get_type().value();

  // Notify view that it should add an index widget for the property
  if (type == core::property::file) {
    emit added_file(index);

  } else if (type == core::property::color) {
    emit added_color(index);
  }

  return index;
}

void property_model::rename_property_in_gui(const QString& oldName,
                                            const QString& newName)
{
  if (auto* item = find_item(this, oldName, 0)) {
    item->setText(newName);
  }
}

void property_model::set_value(const QString& name, QStandardItem* item)
{
  m_manager->set_property(name, item_to_property(item));
}

void property_model::when_data_changed(const QModelIndex& topLeft,
                                       const QModelIndex& bottomRight,
                                       const QVector<int>& roles)
{
  if (m_blockDataChanged) {
    return;
  }

  if (topLeft.parent() == m_customRoot->index()) {
    if (topLeft.column() == 1) {
      if (auto* item = itemFromIndex(topLeft)) {
        const auto sibling = itemFromIndex(item->index().siblingAtColumn(0));
        Q_ASSERT(sibling);

        set_value(sibling->text(), item);
      }
    }
  }
}

}  // namespace tactile::vm
