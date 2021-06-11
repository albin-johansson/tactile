#include "property_model.hpp"

#include "file_value_widget.hpp"
#include "item_model_utils.hpp"
#include "make_property_item.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

PropertyModel::PropertyModel(core::IPropertyManager* manager, QObject* parent)
    : QStandardItemModel{parent}
    , mManager{manager}
    , mCustomRoot{new root_item{tr("Properties")}}
{
  if (!mManager)
  {
    throw TactileError{"Property model requires non-null property manager!"};
  }

  setColumnCount(2);
  appendRow(mCustomRoot);

  // clang-format off
  connect(this, &PropertyModel::dataChanged, this, &PropertyModel::OnDataChanged);
  // clang-format on

  for (const auto& [name, property] : manager->GetProperties())
  {
    const auto index = AddPropertyToGui(name, property, mCustomRoot);
    const auto sibling = index.siblingAtColumn(0);
    itemFromIndex(sibling)->setEditable(false);
  }
}

void PropertyModel::Add(const QString& name, const core::PropertyType type)
{
  mManager->AddProperty(name, type);
  AddExistingPropertyToGui(name);
}

auto PropertyModel::Add(const QString& name, const core::Property& property)
    -> QModelIndex
{
  mManager->AddProperty(name, property);
  return AddExistingPropertyToGui(name);
}

void PropertyModel::ChangeType(const QString& name, core::PropertyType type)
{
  if (auto* item = FindItem(this, name))
  {
    const auto row = item->row();
    removeRow(row, mCustomRoot->index());

    mManager->ChangePropertyType(name, type);
    const auto& property = mManager->GetProperty(name);

    auto* valueItem = MakePropertyItem(mManager->GetProperty(name));
    mCustomRoot->insertRow(row, {new QStandardItem{name}, valueItem});

    emit S_ChangedType(indexFromItem(valueItem), type);
  }
}

void PropertyModel::Rename(const QString& oldName, const QString& newName)
{
  RenamePropertyInGui(oldName, newName);
  mManager->RenameProperty(oldName, newName);
}

void PropertyModel::Remove(const QString& name)
{
  RemovePropertyFromGui(name);
  mManager->RemoveProperty(name);
}

void PropertyModel::SetRootName(const QString& name)
{
  mCustomRoot->setText(name);
}

auto PropertyModel::ContainsProperty(const QString& name) const -> bool
{
  return FindItem(this, name) != nullptr;
}

auto PropertyModel::IsCustomProperty(const QModelIndex& index) const -> bool
{
  return parent(index) == indexFromItem(mCustomRoot);
}

auto PropertyModel::GetProperty(const QString& name) const
    -> const core::Property&
{
  return mManager->GetProperty(name);
}

void PropertyModel::OnAddedProperty(const QString& name)
{
  AddExistingPropertyToGui(name);
}

void PropertyModel::OnAboutToRemoveProperty(const QString& name)
{
  RemovePropertyFromGui(name);
}

void PropertyModel::OnUpdatedProperty(const QString& name)
{
  mBlockDataChanged = true;

  if (auto* item = FindItem(this, name))
  {
    const auto& property = mManager->GetProperty(name);

    const auto sibling = item->index().siblingAtColumn(1);
    Q_ASSERT(sibling.isValid());

    update_item_data(itemFromIndex(sibling), property);

    // We need to notify the view index widget to update its contents
    const auto type = property.Type().value();
    if (type == core::PropertyType::File)
    {
      emit S_UpdatedFile(sibling);
    }
    else if (type == core::PropertyType::Color)
    {
      emit S_UpdatedColor(sibling);
    }
  }

  mBlockDataChanged = false;
}

void PropertyModel::OnChangedPropertyType(const QString& name)
{
  RemovePropertyFromGui(name);
  AddExistingPropertyToGui(name);
}

void PropertyModel::OnRenamedProperty(const QString& oldName,
                                      const QString& newName)
{
  RenamePropertyInGui(oldName, newName);
}

void PropertyModel::RemovePropertyFromGui(const QString& name)
{
  if (auto* item = FindItem(this, name))
  {
    const auto row = item->row();
    removeRow(row, item->parent()->index());
  }
}

auto PropertyModel::AddExistingPropertyToGui(const QString& name) -> QModelIndex
{
  const auto& prop = mManager->GetProperty(name);
  const auto index = AddPropertyToGui(name, prop, mCustomRoot);

  auto* sibling = itemFromIndex(index.siblingAtColumn(0));
  sibling->setEditable(false);

  return index;
}

auto PropertyModel::AddPropertyToGui(const QString& name,
                                     const core::Property& property,
                                     QStandardItem* root) -> QModelIndex
{
  auto* nameItem = new QStandardItem{name};
  auto* valueItem = MakePropertyItem(property);

  root->appendRow({nameItem, valueItem});

  const auto index = indexFromItem(valueItem);
  const auto type = property.Type().value();

  // Notify view that it should add an index widget for the property
  if (type == core::PropertyType::File)
  {
    emit S_AddedFile(index);
  }
  else if (type == core::PropertyType::Color)
  {
    emit S_AddedColor(index);
  }

  return index;
}

void PropertyModel::RenamePropertyInGui(const QString& oldName,
                                        const QString& newName)
{
  if (auto* item = FindItem(this, oldName))
  {
    item->setText(newName);
  }
}

void PropertyModel::SetValue(const QString& name, QStandardItem* item)
{
  mManager->SetProperty(name, item_to_property(item));
}

void PropertyModel::OnDataChanged(const QModelIndex& topLeft,
                                  const QModelIndex& bottomRight,
                                  const QVector<int>& roles)
{
  if (mBlockDataChanged)
  {
    return;
  }

  if (topLeft.parent() == mCustomRoot->index() && topLeft.column() == 1)
  {
    if (auto* item = itemFromIndex(topLeft))
    {
      const auto sibling = itemFromIndex(item->index().siblingAtColumn(0));
      Q_ASSERT(sibling);

      SetValue(sibling->text(), item);
    }
  }
}

}  // namespace tactile::vm
