#include "property_model.hpp"

#include <QDebug>

#include "file_value_widget.hpp"
#include "icons.hpp"
#include "item_model_utils.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::viewmodel {
namespace {

void add_entry(const QString& name,
               QStandardItem* valueItem,
               QStandardItem* root)
{
  const auto rows = root->rowCount();
  root->setChild(rows, 0, new QStandardItem{name});
  root->setChild(rows, 1, valueItem);
}

}  // namespace

property_model::property_model(core::property_manager* manager, QObject* parent)
    : QStandardItemModel{parent}
    , m_manager{manager}
{
  setColumnCount(2);

  m_predefinedRoot = new QStandardItem{tr("Predefined")};
  m_predefinedRoot->setEditable(false);
  m_predefinedRoot->setSelectable(false);
  m_predefinedRoot->setColumnCount(1);
  setItem(0, m_predefinedRoot);

  m_customRoot = new QStandardItem{tr("Custom")};
  m_customRoot->setEditable(false);
  m_customRoot->setSelectable(false);
  m_customRoot->setColumnCount(1);
  setItem(1, m_customRoot);

  connect(this,
          &property_model::itemChanged,
          this,
          &property_model::when_item_changed);
}

void property_model::clear_predefined()
{
  removeRows(0, m_predefinedRoot->rowCount(), m_predefinedRoot->index());
}

auto property_model::add_predefined(const QString& name,
                                    const core::property& property,
                                    bool readOnly) -> QModelIndex
{
  const auto index = add_property(name, property, m_predefinedRoot);
  itemFromIndex(index)->setEnabled(!readOnly);
  itemFromIndex(index.siblingAtColumn(0))->setEnabled(false);
  return index;
}

auto property_model::add(const QString& name, core::property::type type)
    -> QModelIndex
{
  m_manager->add_property(name, type);

  const auto index =
      add_property(name, m_manager->get_property(name), m_customRoot);
  itemFromIndex(index.siblingAtColumn(0))->setEditable(false);

  return index;
}

void property_model::rename(const QString& oldName, const QString& newName)
{
  if (auto* item = find_item(this, oldName, 0)) {
    item->setText(newName);
  }
  m_manager->rename_property(oldName, newName);
}

void property_model::remove(const QString& name)
{
  if (auto* item = find_item(this, name, 0)) {
    removeRow(item->row(), item->parent()->index());
  }
  m_manager->remove_property(name);
}

void property_model::set_predefined_name(const QString& name)
{
  m_predefinedRoot->setText(name);
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

auto property_model::add_property(const QString& name,
                                  const core::property& property,
                                  QStandardItem* root) -> QModelIndex
{
  switch (property.get_type().value()) {
    case core::property::string:
      return add_string(name, property, root);

    case core::property::integer:
      return add_int(name, property, root);

    case core::property::floating:
      return add_float(name, property, root);

    case core::property::boolean:
      return add_bool(name, property, root);

    case core::property::file:
      return add_file(name, property, root);

    case core::property::color:
      return add_color(name, property, root);

    case core::property::object:
      return add_object(name, property, root);

    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

auto property_model::add_string(const QString& name,
                                const core::property& property,
                                QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new string_item{};
  valueItem->setData(property.as<QString>(), Qt::EditRole);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_string(index);
  return index;
}

auto property_model::add_int(const QString& name,
                             const core::property& property,
                             QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new int_item{};
  valueItem->setData(property.as<int>(), Qt::EditRole);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_int(index);
  return index;
}

auto property_model::add_float(const QString& name,
                               const core::property& property,
                               QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new float_item{};
  valueItem->setData(property.as<double>(), Qt::EditRole);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_float(index);
  return index;
}

auto property_model::add_bool(const QString& name,
                              const core::property& property,
                              QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new bool_item{};
  valueItem->setData(property.as<bool>() ? Qt::Checked : Qt::Unchecked,
                     Qt::CheckStateRole);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_bool(index);
  return index;
}

auto property_model::add_object(const QString& name,
                                const core::property& property,
                                QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new object_item{};

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_object(index);
  return index;
}

auto property_model::add_color(const QString& name,
                               const core::property& property,
                               QStandardItem* root) -> QModelIndex
{
  Q_ASSERT(property.is<QColor>());

  auto* valueItem = new color_item{};
  valueItem->setData(property.as<QColor>(), property_item_role::color);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_color(index);
  return index;
}

auto property_model::add_file(const QString& name,
                              const core::property& property,
                              QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new file_item{};
  valueItem->setEditable(false);
  valueItem->setData(gui::icons::copy(), Qt::DecorationRole);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_file(index);
  return index;
}

void property_model::update_name(const QString& oldName, const QString& newName)
{
  m_manager->rename_property(oldName, newName);
}

void property_model::set_value(const QString& name, QStandardItem* item)
{
  switch (static_cast<viewmodel::item_type>(item->type())) {
    case item_type::string: {
      m_manager->set_property(name, item->data(Qt::EditRole).value<QString>());
      break;
    }
    case item_type::integer: {
      m_manager->set_property(name, item->data(Qt::EditRole).value<int>());
      break;
    }
    case item_type::floating: {
      m_manager->set_property(name, item->data(Qt::EditRole).value<double>());
      break;
    }
    case item_type::boolean: {
      m_manager->set_property(name,
                              item->data(Qt::CheckStateRole).value<bool>());
      break;
    }
    case item_type::file: {
      const auto path =
          item->data(viewmodel::property_item_role::path).value<QString>();
      m_manager->set_property(name, QFileInfo{path});
      break;
    }
    case item_type::color: {
      const auto color =
          item->data(viewmodel::property_item_role::color).value<QColor>();
      m_manager->set_property(name, color);
      break;
    }
    case item_type::object:
      break;  // TODO
  }
}

void property_model::when_item_changed(QStandardItem* item)
{
  if (item->parent() == m_customRoot && item->column() == 1) {
    const auto name = m_customRoot->child(item->row(), 0)->text();
    set_value(name, item);
  }
}

}  // namespace tactile::viewmodel
