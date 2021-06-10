#include "layer_widget.hpp"

#include "icons.hpp"
#include "init_ui.hpp"
#include "layer_item_context_menu.hpp"
#include "layer_list_view.hpp"
#include "layer_model.hpp"
#include "layer_widget_context_menu.hpp"
#include "map_document.hpp"
#include "ui_layer_widget.h"

namespace tactile {

// FIXME status bar layer combobox is not updated directly when layer name is
// changed

LayerWidget::LayerWidget(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::LayerWidget>(this)}
    , mView{new LayerListView{this}}
    , mAddLayerMenu{new AddLayerContextMenu{this}}
    , mWidgetMenu{new LayerWidgetContextMenu{mAddLayerMenu, this}}
    , mItemMenu{new LayerItemContextMenu{this}}
{
  mUi->gridLayout->addWidget(mView, 0, 1);
  mUi->gridLayout->removeWidget(mUi->opacitySlider);
  mUi->gridLayout->addWidget(mUi->opacitySlider, 0, 2);

  mUi->removeLayerButton->setEnabled(false);
  mUi->duplicateButton->setEnabled(false);
  mUi->upButton->setEnabled(false);
  mUi->downButton->setEnabled(false);
  mUi->visibleButton->setEnabled(false);
  mUi->opacitySlider->setEnabled(false);

  // clang-format off
  connect(mUi->opacitySlider, &QSlider::valueChanged,
          this, &LayerWidget::OnOpacitySliderValueChanged);

  connect(mUi->newLayerButton, &QPushButton::pressed,
          this, &LayerWidget::OnNewLayerButtonPressed);

  connect(mUi->removeLayerButton, &QPushButton::pressed,
          this, &LayerWidget::OnRemoveLayerButtonPressed);

  connect(mUi->upButton, &QPushButton::pressed,
          this, &LayerWidget::OnUpButtonPressed);

  connect(mUi->downButton, &QPushButton::pressed,
          this, &LayerWidget::OnDownButtonPressed);

  connect(mUi->duplicateButton, &QPushButton::pressed,
          this, &LayerWidget::OnDuplicateButtonPressed);

  connect(mUi->visibleButton, &QPushButton::toggled,
          this, &LayerWidget::OnVisibleButtonToggled);

  connect(mView, &QWidget::customContextMenuRequested,
          this, &LayerWidget::OnSpawnContextMenu);

  connect(mView, &LayerListView::S_SelectionChanged,
          this, &LayerWidget::OnViewChangedSelection);

  connect(mView, &LayerListView::S_ChangedName,
          this, &LayerWidget::OnViewChangedName);

  connect(mAddLayerMenu, &AddLayerContextMenu::S_AddTileLayer,
          this, &LayerWidget::OnNewTileLayerRequested);

  connect(mAddLayerMenu, &AddLayerContextMenu::S_AddObjectLayer,
          this, &LayerWidget::OnNewObjectLayerRequested);

  connect(mItemMenu, &LayerItemContextMenu::S_ToggleVisibility,
          mUi->visibleButton, &QPushButton::toggle);

  connect(mItemMenu, &LayerItemContextMenu::S_MoveLayerUp,
          mUi->upButton, &QPushButton::click);

  connect(mItemMenu, &LayerItemContextMenu::S_MoveLayerDown,
          mUi->downButton, &QPushButton::click);

  connect(mItemMenu, &LayerItemContextMenu::S_DuplicateLayer,
          mUi->duplicateButton, &QPushButton::click);

  connect(mItemMenu, &LayerItemContextMenu::S_RemoveLayer,
          mUi->removeLayerButton, &QPushButton::click);

  connect(mItemMenu, &LayerItemContextMenu::S_ShowProperties,
          [this] { mModel->show_properties(mView->currentIndex()); });

  // clang-format on
}

LayerWidget::~LayerWidget() noexcept = default;

void LayerWidget::OnSwitchedMap(not_null<core::map_document*> document)
{
  Q_ASSERT(document);

  mModel = std::make_unique<vm::layer_model>(document);
  Q_ASSERT(!mModel->parent());

  mView->setModel(mModel.get());
  Q_ASSERT(!mModel->parent());

  // clang-format off
  connect(mModel.get(), &vm::layer_model::changed_opacity,
          this, &LayerWidget::OnChangedLayerOpacity);

  connect(mModel.get(), &vm::layer_model::changed_name,
          this, &LayerWidget::OnChangedLayerName);

  connect(mModel.get(), &vm::layer_model::changed_visibility,
          this, &LayerWidget::OnChangedLayerVisibility);

  connect(mModel.get(), &vm::layer_model::selected_layer,
          this, &LayerWidget::OnSelectedLayer);
  // clang-format on
}

void LayerWidget::UpdateActions(const maybe<QModelIndex>& selected)
{
  if (!mModel)
  {
    return;
  }

  mUi->opacitySlider->setEnabled(selected.has_value());
  mUi->duplicateButton->setEnabled(selected.has_value());
  mUi->visibleButton->setEnabled(selected.has_value());

  const auto nLayers = mModel->rowCount();

  maybe<int> index;
  if (selected)
  {
    index = selected->row();
  }

  mUi->upButton->setEnabled(index && index != 0);
  mUi->downButton->setEnabled(index && index != nLayers - 1);
  mUi->removeLayerButton->setEnabled(nLayers != 1);

  if (selected)
  {
    QSignalBlocker opacityBlocker{mUi->opacitySlider};
    mUi->opacitySlider->setValue(
        static_cast<int>(mModel->opacity(*selected) * 100.0));

    mUi->visibleButton->setIcon(mModel->visible(*selected) ? IconVisible()
                                                           : IconInvisible());
  }

  mItemMenu->SetVisibilityEnabled(mUi->visibleButton->isEnabled());
  mItemMenu->SetRemoveEnabled(mUi->removeLayerButton->isEnabled());
  mItemMenu->SetMoveUpEnabled(mUi->upButton->isEnabled());
  mItemMenu->SetMoveDownEnabled(mUi->downButton->isEnabled());
}

void LayerWidget::OnSpawnContextMenu(const QPoint& pos)
{
  if (mView->selectionModel()->selection().empty())
  {
    mWidgetMenu->exec(mapToGlobal(pos));
  }
  else
  {
    mItemMenu->exec(mapToGlobal(pos));
  }
}

void LayerWidget::OnViewChangedSelection(const maybe<QModelIndex> selected,
                                         const maybe<QModelIndex>)
{
  if (selected)
  {
    mModel->select(*selected);
  }
}

void LayerWidget::OnViewChangedName(const QModelIndex& index,
                                    const QString& name)
{
  mModel->set_name(index, name);
}

void LayerWidget::OnNewTileLayerRequested()
{
  Q_ASSERT(mModel);
  mModel->add_tile_layer();
}

void LayerWidget::OnNewObjectLayerRequested()
{
  Q_ASSERT(mModel);
  mModel->add_object_layer();
}

void LayerWidget::OnChangedLayerOpacity(const layer_id, double opacity)
{
  QSignalBlocker blocker{mUi->opacitySlider};
  mUi->opacitySlider->setValue(static_cast<int>(opacity * 100.0));
}

void LayerWidget::OnChangedLayerName(const layer_id id, const QString& name)
{
  QSignalBlocker blocker{mView};
  mModel->itemFromIndex(mModel->index_of(id).value())->setText(name);
}

void LayerWidget::OnChangedLayerVisibility(const layer_id id, bool visible)
{
  mUi->visibleButton->setIcon(visible ? IconVisible() : IconInvisible());
}

void LayerWidget::OnSelectedLayer(const layer_id id, const core::layer& layer)
{
  const auto index = mModel->index_of(id).value();

  mView->SelectQuietly(index);
  UpdateActions(index);
}

void LayerWidget::OnOpacitySliderValueChanged(const int value)
{
  mModel->set_opacity(mView->currentIndex(),
                      static_cast<double>(value) / 100.0);
}

void LayerWidget::OnNewLayerButtonPressed()
{
  auto pos = mUi->newLayerButton->pos();

  const auto size = mUi->newLayerButton->size() / 2.0;
  pos += QPoint{size.width(), size.height()};

  mAddLayerMenu->exec(mapToGlobal(pos));
}

void LayerWidget::OnRemoveLayerButtonPressed()
{
  mModel->remove(mView->currentIndex());
  UpdateActions(mView->currentIndex());
}

void LayerWidget::OnUpButtonPressed()
{
  const auto current = mView->currentIndex();
  const auto next = current.siblingAtRow(current.row() - 1);

  mModel->move_up(current);
  mView->setCurrentIndex(next);
}

void LayerWidget::OnDownButtonPressed()
{
  const auto current = mView->currentIndex();
  const auto next = current.siblingAtRow(current.row() + 1);

  mModel->move_down(current);
  mView->setCurrentIndex(next);
}

void LayerWidget::OnDuplicateButtonPressed()
{
  mModel->duplicate(mView->currentIndex());
}

void LayerWidget::OnVisibleButtonToggled(const bool visible)
{
  mModel->set_visible(mView->currentIndex(), visible);
  mUi->visibleButton->setIcon(visible ? IconVisible() : IconInvisible());
}

}  // namespace tactile
