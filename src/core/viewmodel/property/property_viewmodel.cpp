#include "property_viewmodel.hpp"

#include <QDebug>

#include "file_value_widget.hpp"
#include "icons.hpp"
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

property_viewmodel::property_viewmodel(core::property_manager* manager,
                                       QObject* parent)
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
          &property_viewmodel::itemChanged,
          this,
          &property_viewmodel::when_item_changed);
}

void property_viewmodel::clear_predefined()
{
  removeRows(0, m_predefinedRoot->rowCount(), m_predefinedRoot->index());
}

auto property_viewmodel::add_predefined(const QString& name,
                                        const core::property& property,
                                        bool readOnly) -> QModelIndex
{
  const auto index = add_property(name, property, m_predefinedRoot);
  itemFromIndex(index)->setEnabled(!readOnly);
  itemFromIndex(index.siblingAtColumn(0))->setEnabled(false);
  return index;
}

auto property_viewmodel::add(const QString& name, core::property::type type)
    -> QModelIndex
{
  m_manager->add_property(name, type);
  return add_property(name, m_manager->get_property(name), m_customRoot);
}

void property_viewmodel::set_predefined_name(const QString& name)
{
  m_predefinedRoot->setText(name);
}

auto property_viewmodel::add_property(const QString& name,
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

auto property_viewmodel::add_string(const QString& name,
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

auto property_viewmodel::add_int(const QString& name,
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

auto property_viewmodel::add_float(const QString& name,
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

auto property_viewmodel::add_bool(const QString& name,
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

auto property_viewmodel::add_object(const QString& name,
                                    const core::property& property,
                                    QStandardItem* root) -> QModelIndex
{
  auto* valueItem = new object_item{};

  add_entry(name, valueItem, root);
  const auto index = indexFromItem(valueItem);

  emit added_object(index);
  return index;
}

auto property_viewmodel::add_color(const QString& name,
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

auto property_viewmodel::add_file(const QString& name,
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

void property_viewmodel::when_item_changed(QStandardItem* item)
{
  if (item->parent() == m_customRoot) {
    const auto name = m_customRoot->child(item->row(), 0)->text();

    switch (static_cast<viewmodel::item_type>(item->type())) {
      case item_type::string: {
        m_manager->set_property(name,
                                item->data(Qt::EditRole).value<QString>());
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
}

}  // namespace tactile::viewmodel
