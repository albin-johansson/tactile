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

void property_viewmodel::add_predefined(const QString& name,
                                        const core::property& property)
{
  add_property(name, property, m_predefinedRoot);
}

void property_viewmodel::add(const QString& name, core::property::type type)
{
  m_manager->add_property(name, type);
  add_property(name, m_manager->get_property(name), m_customRoot);
}

void property_viewmodel::set_predefined_name(const QString& name)
{
  m_predefinedRoot->setText(name);
}

void property_viewmodel::add_property(const QString& name,
                                      const core::property& property,
                                      QStandardItem* root)
{
  switch (property.get_type().value()) {
    case core::property::string: {
      add_string(name, property, root);
      break;
    }
    case core::property::integer: {
      add_int(name, property, root);
      break;
    }
    case core::property::floating: {
      add_float(name, property, root);
      break;
    }
    case core::property::boolean: {
      add_bool(name, property, root);
      break;
    }
    case core::property::file: {
      add_file(name, property, root);
      break;
    }
    case core::property::color: {
      add_color(name, property, root);
      break;
    }
    case core::property::object: {
      add_object(name, property, root);
      break;
    }
    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

void property_viewmodel::add_string(const QString& name,
                                    const core::property& property,
                                    QStandardItem* root)
{
  auto* valueItem = new string_item{};
  valueItem->setData(property.as<QString>(), Qt::EditRole);

  add_entry(name, valueItem, root);
  emit added_string(indexFromItem(valueItem));
}

void property_viewmodel::add_int(const QString& name,
                                 const core::property& property,
                                 QStandardItem* root)
{
  auto* valueItem = new int_item{};
  valueItem->setData(property.as<int>(), Qt::EditRole);

  add_entry(name, valueItem, root);
  emit added_int(indexFromItem(valueItem));
}

void property_viewmodel::add_float(const QString& name,
                                   const core::property& property,
                                   QStandardItem* root)
{
  auto* valueItem = new float_item{};
  valueItem->setData(property.as<double>(), Qt::EditRole);

  add_entry(name, valueItem, root);
  emit added_float(indexFromItem(valueItem));
}

void property_viewmodel::add_bool(const QString& name,
                                  const core::property& property,
                                  QStandardItem* root)
{
  auto* valueItem = new bool_item{};
  valueItem->setData(property.as<bool>() ? Qt::Checked : Qt::Unchecked,
                     Qt::CheckStateRole);

  add_entry(name, valueItem, root);
  emit added_bool(indexFromItem(valueItem));
}

void property_viewmodel::add_object(const QString& name,
                                    const core::property& property,
                                    QStandardItem* root)
{
  auto* valueItem = new object_item{};
  add_entry(name, valueItem, root);
  emit added_object(indexFromItem(valueItem));
}

void property_viewmodel::add_color(const QString& name,
                                   const core::property& property,
                                   QStandardItem* root)
{
  Q_ASSERT(property.is<QColor>());

  auto* valueItem = new color_item{};
  valueItem->setData(property.as<QColor>(), property_item_role::color);

  add_entry(name, valueItem, root);
  emit added_color(indexFromItem(valueItem));
}

void property_viewmodel::add_file(const QString& name,
                                  const core::property& property,
                                  QStandardItem* root)
{
  auto* valueItem = new file_item{};
  valueItem->setEditable(false);
  valueItem->setData(gui::icons::copy(), Qt::DecorationRole);

  add_entry(name, valueItem, root);
  emit added_file(indexFromItem(valueItem));
}

void property_viewmodel::when_item_changed(QStandardItem* item)
{
  if (item->parent() == m_customRoot) {
    const auto row = item->row();
    const auto name = m_customRoot->child(row, 0)->text();
    qDebug() << "Changed value of" << name << "property...";
  }
}

}  // namespace tactile::viewmodel
