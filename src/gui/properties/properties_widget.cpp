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

namespace tactile {

PropertiesWidget::PropertiesWidget(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::PropertiesWidget>(this)}
    , mContextMenu{new PropertyContextMenu{this}}
    , mView{new PropertyTreeView{this}}
{
  mUi->gridLayout->addWidget(mView);
  mUi->removeButton->setEnabled(false);
  mUi->renameButton->setEnabled(false);
  mContextMenu->DisableAll();
  mContextMenu->SetAddEnabled(true);

  // clang-format off
  connect(mUi->addButton, &QPushButton::pressed, this, &PropertiesWidget::OnNewPropertyRequested);
  connect(mUi->removeButton, &QPushButton::pressed, this, &PropertiesWidget::OnRemovePropertyRequested);
  connect(mUi->renameButton, &QPushButton::pressed, this, &PropertiesWidget::OnRenamePropertyRequested);

  connect(mView, &PropertyTreeView::S_SpawnContextMenu, this, &PropertiesWidget::OnSpawnContextMenu);
  connect(mView, &PropertyTreeView::S_SelectionChanged, this, &PropertiesWidget::OnSelectionChanged);
  connect(mView, &QTreeView::doubleClicked, this, &PropertiesWidget::OnDoubleClicked);

  connect(mContextMenu, &PropertyContextMenu::S_Copy, this, &PropertiesWidget::OnCopyPropertyRequested);
  connect(mContextMenu, &PropertyContextMenu::S_Paste, this, &PropertiesWidget::OnPastePropertyRequested);
  connect(mContextMenu, &PropertyContextMenu::S_Add, this, &PropertiesWidget::OnNewPropertyRequested);
  connect(mContextMenu, &PropertyContextMenu::S_Remove, this, &PropertiesWidget::OnRemovePropertyRequested);
  connect(mContextMenu, &PropertyContextMenu::S_Rename, this, &PropertiesWidget::OnRenamePropertyRequested);
  connect(mContextMenu, &PropertyContextMenu::S_ChangeType, this, &PropertiesWidget::OnChangeTypeRequested);
  // clang-format on
}

PropertiesWidget::~PropertiesWidget() noexcept = default;

void PropertiesWidget::OnAddedProperty(const QString& name)
{
  mModel->OnAddedProperty(name);
}

void PropertiesWidget::OnAboutToRemoveProperty(const QString& name)
{
  mModel->OnAboutToRemoveProperty(name);
}

void PropertiesWidget::OnUpdatedProperty(const QString& name)
{
  mModel->OnUpdatedProperty(name);
}

void PropertiesWidget::OnRenamedProperty(const QString& oldName,
                                         const QString& newName)
{
  mModel->OnRenamedProperty(oldName, newName);
}

void PropertiesWidget::OnUpdatedPropertyContext(
    NotNull<core::IPropertyManager*> context,
    const QStringView name)
{
  ChangeModel(context);
  mModel->SetRootName(name.toString());
}

void PropertiesWidget::OnChangedPropertyType(const QString& name)
{
  mModel->OnChangedPropertyType(name);
}

void PropertiesWidget::OnSelectionChanged(Maybe<QModelIndex> index)
{
  mContextMenu->DisableAll();

  mContextMenu->SetAddEnabled(true);
  mContextMenu->SetPasteEnabled(mNameCopy.has_value());

  mUi->addButton->setEnabled(true);
  mUi->removeButton->setEnabled(false);
  mUi->renameButton->setEnabled(false);

  if (!index || !mModel->itemFromIndex(*index))
  {
    return;
  }

  const auto isCustom = mModel->IsCustomProperty(*index);
  mUi->removeButton->setEnabled(isCustom);
  mUi->renameButton->setEnabled(isCustom);

  mContextMenu->SetCopyEnabled(isCustom);
  mContextMenu->SetChangeTypeEnabled(isCustom);
  mContextMenu->SetRemoveEnabled(mUi->removeButton->isEnabled());
  mContextMenu->SetRenameEnabled(mUi->renameButton->isEnabled());
  if (isCustom)
  {
    const auto& property = mModel->GetProperty(PropertyName(*index));
    mContextMenu->SetCurrentType(property.Type().value());
  }
}

void PropertiesWidget::OnCopyPropertyRequested()
{
  const auto currentName = CurrentPropertyName();
  mNameCopy = currentName;
  mPropertyCopy = mModel->GetProperty(currentName);
}

void PropertiesWidget::OnPastePropertyRequested()
{
  const auto& name = mNameCopy.value();
  if (!mModel->ContainsProperty(name))
  {
    mModel->Add(name, mPropertyCopy.value());
    mContextMenu->SetPasteEnabled(false);
  }
}

void PropertiesWidget::OnNewPropertyRequested()
{
  AddPropertyDialog::Spawn(
      [this](const QString& name, const core::PropertyType type) {
        mModel->Add(name, type);
      },
      mModel.get(),
      this);
}

void PropertiesWidget::OnRemovePropertyRequested()
{
#ifdef QT_DEBUG
  const auto index = mView->currentIndex();
  Q_ASSERT(mModel->IsCustomProperty(index));
#endif  // QT_DEBUG

  mModel->Remove(CurrentPropertyName());
}

void PropertiesWidget::OnRenamePropertyRequested()
{
  const auto oldName = CurrentPropertyName();
  if (const auto newName = ChangePropertyNameDialog::Spawn(mModel.get()))
  {
    mModel->Rename(oldName, *newName);
  }
}

void PropertiesWidget::OnChangeTypeRequested(const core::PropertyType type)
{
  mModel->ChangeType(CurrentPropertyName(), type);
}

void PropertiesWidget::OnDoubleClicked()
{
  const auto index = mView->currentIndex();
  const auto* item = mModel->itemFromIndex(index);
  if (index.column() == 0 && index.parent().isValid() && item->isEnabled())
  {
    const auto oldName = item->text();
    if (const auto name = ChangePropertyNameDialog::Spawn(mModel.get()))
    {
      mModel->Rename(oldName, *name);
    }
  }
}

void PropertiesWidget::OnSpawnContextMenu(const QPoint& pos)
{
  mContextMenu->exec(pos);
}

void PropertiesWidget::ChangeModel(NotNull<core::IPropertyManager*> manager)
{
  Q_ASSERT(manager);

  mView->collapseAll();

  mModel = std::make_unique<vm::PropertyModel>(manager);
  Q_ASSERT(!mModel->parent());

  mView->setModel(mModel.get());
  Q_ASSERT(!mModel->parent());

  // clang-format off
  connect(mModel.get(), &vm::PropertyModel::S_ChangedType, mView, &PropertyTreeView::OnChangedType);
  connect(mModel.get(), &vm::PropertyModel::S_AddedFile, mView, &PropertyTreeView::OnFileAdded);
  connect(mModel.get(), &vm::PropertyModel::S_AddedColor, mView, &PropertyTreeView::OnColorAdded);
  connect(mModel.get(), &vm::PropertyModel::S_UpdatedFile, mView, &PropertyTreeView::OnFileUpdated);
  connect(mModel.get(), &vm::PropertyModel::S_UpdatedColor, mView, &PropertyTreeView::OnColorUpdated);
  // clang-format on

  mView->setFirstColumnSpanned(0, mView->rootIndex(), true);
  mView->RestoreItemWidgets();

  mContextMenu->DisableAll();
  mContextMenu->SetAddEnabled(true);

  mView->expandAll();
}

auto PropertiesWidget::PropertyName(const QModelIndex& index) const -> QString
{
  const auto* item = mModel->itemFromIndex(index);
  Q_ASSERT(item);

  if (item->column() == 0)
  {
    return item->text();
  }
  else
  {
    return mModel->itemFromIndex(index.siblingAtColumn(0))->text();
  }
}

auto PropertiesWidget::CurrentPropertyName() const -> QString
{
  const auto index = mView->currentIndex();
  Q_ASSERT(index.isValid());
  return PropertyName(index);
}

}  // namespace tactile
