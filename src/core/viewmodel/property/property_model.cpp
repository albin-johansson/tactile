#include "property_model.hpp"

#include "file_value_widget.hpp"
#include "icons.hpp"
#include "item_model_utils.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {
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
  if (!m_manager) {
    throw tactile_error{"property_model requires non-null property manager!"};
  }

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

  // clang-format off
  connect(this, &property_model::itemChanged,
          this, &property_model::when_item_changed);
  // clang-format on

  for (const auto& [name, property] : manager->properties()) {
    const auto index = add_property(name, property, m_customRoot);
    itemFromIndex(index.siblingAtColumn(0))->setEditable(false);
  }
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

auto property_model::add(const QString& name, const core::property& property)
    -> QModelIndex
{
  Q_ASSERT(!contains_property(name));

  m_manager->add_property(name, property.get_type().value());
  m_manager->set_property(name, property);

  const auto index =
      add_property(name, m_manager->get_property(name), m_customRoot);
  itemFromIndex(index.siblingAtColumn(0))->setEditable(false);

  return index;
}

void property_model::change_type(const QString& name,
                                 const core::property::type type)
{
  if (auto* item = find_item(this, name, 0)) {
    const auto row = item->row();

    m_manager->remove_property(name);
    m_manager->add_property(name, type);

    auto& property = m_manager->get_property(name);
    property.set_default(type);

    removeRow(row, m_customRoot->index());

    auto* valueItem = make_item(property);
    m_customRoot->insertRow(row, {new QStandardItem{name}, valueItem});

    emit changed_type(indexFromItem(valueItem), type);
  }
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

auto property_model::add_property(const QString& name,
                                  const core::property& property,
                                  QStandardItem* root) -> QModelIndex
{
  auto* valueItem = make_item(property);

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  if (const auto type = property.get_type().value();
      type == core::property::file) {
    emit added_file(index);
  } else if (type == core::property::color) {
    emit added_color(index);
  }

  return index;
}

void property_model::set_value(const QString& name, QStandardItem* item)
{
  switch (static_cast<vm::item_type>(item->type())) {
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
          item->data(vm::property_item_role::path).value<QString>();
      m_manager->set_property(name, QFileInfo{path});
      break;
    }
    case item_type::color: {
      const auto color =
          item->data(vm::property_item_role::color).value<QColor>();
      m_manager->set_property(name, color);
      break;
    }
    case item_type::object:
      break;  // TODO
  }
}

auto property_model::make_item(const core::property& property) -> QStandardItem*
{
  switch (property.get_type().value()) {
    case core::property::string: {
      auto* item = new string_item{};
      item->setData(property.as<QString>(), Qt::EditRole);
      return item;
    }
    case core::property::integer: {
      auto* item = new int_item{};
      item->setData(property.as<int>(), Qt::EditRole);
      return item;
    }
    case core::property::floating: {
      auto* item = new float_item{};
      item->setData(property.as<double>(), Qt::EditRole);
      return item;
    }
    case core::property::boolean: {
      auto* item = new bool_item{};
      item->setData(property.as<bool>() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      return item;
    }
    case core::property::file: {
      auto* item = new file_item{};
      item->setData(gui::icons::copy(), Qt::DecorationRole);
      item->setData(property.as<QFileInfo>().filePath(),
                    vm::property_item_role::path);
      return item;
    }
    case core::property::color: {
      auto* item = new color_item{};
      item->setData(property.as<QColor>(), property_item_role::color);
      return item;
    }
    case core::property::object:
      return new object_item{};

    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

void property_model::when_item_changed(QStandardItem* item)
{
  if (item->parent() == m_customRoot && item->column() == 1) {
    const auto name = m_customRoot->child(item->row(), 0)->text();
    set_value(name, item);
  }
}

}  // namespace tactile::vm
