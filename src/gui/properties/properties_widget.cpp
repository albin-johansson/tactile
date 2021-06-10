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
  connect(mUi->addButton, &QPushButton::pressed,
          this, &PropertiesWidget::OnNewPropertyRequested);

  connect(mUi->removeButton, &QPushButton::pressed,
          this, &PropertiesWidget::OnRemovePropertyRequested);

  connect(mUi->renameButton, &QPushButton::pressed,
          this, &PropertiesWidget::OnRenamePropertyRequested);

  connect(mView, &PropertyTreeView::S_SpawnContextMenu,
          this, &PropertiesWidget::OnSpawnContextMenu);

  connect(mView, &PropertyTreeView::S_SelectionChanged,
          this, &PropertiesWidget::OnSelectionChanged);

  connect(mView, &QTreeView::doubleClicked,
          this, &PropertiesWidget::OnDoubleClicked);

  connect(mContextMenu, &PropertyContextMenu::S_Copy,
          this, &PropertiesWidget::OnCopyPropertyRequested);

  connect(mContextMenu, &PropertyContextMenu::S_Paste,
          this, &PropertiesWidget::OnPastePropertyRequested);

  connect(mContextMenu, &PropertyContextMenu::S_Add,
          this, &PropertiesWidget::OnNewPropertyRequested);

  connect(mContextMenu, &PropertyContextMenu::S_Remove,
          this, &PropertiesWidget::OnRemovePropertyRequested);

  connect(mContextMenu, &PropertyContextMenu::S_Rename,
          this, &PropertiesWidget::OnRenamePropertyRequested);

  connect(mContextMenu, &PropertyContextMenu::S_ChangeType,
          this, &PropertiesWidget::OnChangeTypeRequested);
  // clang-format on
}

PropertiesWidget::~PropertiesWidget() noexcept = default;

void PropertiesWidget::ShowMap(not_null<core::IPropertyManager*> manager)
{
  ChangeModel(manager);
  mModel->set_root_name(tr("Map"));
}

void PropertiesWidget::ShowLayer(not_null<core::IPropertyManager*> manager)
{
  ChangeModel(manager);
  mModel->set_root_name(tr("Layer"));
}

void PropertiesWidget::OnAddedProperty(const QString& name)
{
  mModel->added_property(name);
}

void PropertiesWidget::OnAboutToRemoveProperty(const QString& name)
{
  mModel->about_to_remove_property(name);
}

void PropertiesWidget::OnUpdatedProperty(const QString& name)
{
  mModel->updated_property(name);
}

void PropertiesWidget::OnRenamedProperty(const QString& oldName,
                                         const QString& newName)
{
  mModel->renamed_property(oldName, newName);
}

void PropertiesWidget::OnChangedPropertyType(const QString& name)
{
  mModel->changed_property_type(name);
}

void PropertiesWidget::OnSelectionChanged(maybe<QModelIndex> index)
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

  const auto isCustom = mModel->is_custom_property(*index);
  mUi->removeButton->setEnabled(isCustom);
  mUi->renameButton->setEnabled(isCustom);

  mContextMenu->SetCopyEnabled(isCustom);
  mContextMenu->SetChangeTypeEnabled(isCustom);
  mContextMenu->SetRemoveEnabled(mUi->removeButton->isEnabled());
  mContextMenu->SetRenameEnabled(mUi->renameButton->isEnabled());
  if (isCustom)
  {
    const auto& property = mModel->get_property(PropertyName(*index));
    mContextMenu->SetCurrentType(property.type().value());
  }
}

void PropertiesWidget::OnCopyPropertyRequested()
{
  const auto currentName = CurrentPropertyName();
  mNameCopy = currentName;
  mPropertyCopy = mModel->get_property(currentName);
}

void PropertiesWidget::OnPastePropertyRequested()
{
  const auto& name = mNameCopy.value();
  if (!mModel->contains_property(name))
  {
    mModel->add(name, mPropertyCopy.value());
    mContextMenu->SetPasteEnabled(false);
  }
}

void PropertiesWidget::OnNewPropertyRequested()
{
  AddPropertyDialog::Spawn(
      [this](const QString& name, const core::property_type type) {
        mModel->add(name, type);
      },
      mModel.get(),
      this);
}

void PropertiesWidget::OnRemovePropertyRequested()
{
#ifdef QT_DEBUG
  const auto index = mView->currentIndex();
  Q_ASSERT(mModel->is_custom_property(index));
#endif  // QT_DEBUG

  mModel->remove(CurrentPropertyName());
}

void PropertiesWidget::OnRenamePropertyRequested()
{
  const auto oldName = CurrentPropertyName();
  if (const auto newName = ChangePropertyNameDialog::Spawn(mModel.get()))
  {
    mModel->rename(oldName, *newName);
  }
}

void PropertiesWidget::OnChangeTypeRequested(core::property_type type)
{
  mModel->change_type(CurrentPropertyName(), type);
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
      mModel->rename(oldName, *name);
    }
  }
}

void PropertiesWidget::OnSpawnContextMenu(const QPoint& pos)
{
  mContextMenu->exec(pos);
}

void PropertiesWidget::ChangeModel(not_null<core::IPropertyManager*> manager)
{
  Q_ASSERT(manager);

  mView->collapseAll();

  mModel = std::make_unique<vm::property_model>(manager);
  Q_ASSERT(!mModel->parent());

  mView->setModel(mModel.get());
  Q_ASSERT(!mModel->parent());

  // clang-format off
  connect(mModel.get(), &vm::property_model::changed_type,
          mView,        &PropertyTreeView::OnChangedType);

  connect(mModel.get(), &vm::property_model::added_file,
          mView,        &PropertyTreeView::OnFileAdded);

  connect(mModel.get(), &vm::property_model::added_color,
          mView,        &PropertyTreeView::OnColorAdded);

  connect(mModel.get(), &vm::property_model::updated_file,
          mView,        &PropertyTreeView::OnFileUpdated);

  connect(mModel.get(), &vm::property_model::updated_color,
          mView,        &PropertyTreeView::OnColorUpdated);
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
