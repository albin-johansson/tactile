#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(LayerWidget)
TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)
TACTILE_FORWARD_DECLARE(tactile::core, ILayer)
TACTILE_FORWARD_DECLARE(tactile::vm, LayerModel)
TACTILE_FORWARD_DECLARE(tactile, LayerListView)
TACTILE_FORWARD_DECLARE(tactile, LayerItem)
TACTILE_FORWARD_DECLARE(tactile, AddLayerContextMenu)
TACTILE_FORWARD_DECLARE(tactile, LayerItemContextMenu)
TACTILE_FORWARD_DECLARE(tactile, LayerWidgetContextMenu)

namespace tactile {

class LayerWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit LayerWidget(QWidget* visible = nullptr);

  ~LayerWidget() noexcept override;

 public slots:
  void OnSwitchedMap(not_null<core::MapDocument*> document);

 private:
  Unique<Ui::LayerWidget> mUi;
  LayerListView* mView{};
  AddLayerContextMenu* mAddLayerMenu{};
  LayerWidgetContextMenu* mWidgetMenu{};
  LayerItemContextMenu* mItemMenu{};
  Unique<vm::LayerModel> mModel;

  void UpdateActions(const Maybe<QModelIndex>& selected);

 private slots:
  void OnChangedLayerOpacity(layer_id id, double opacity);
  void OnChangedLayerName(layer_id id, const QString& name);
  void OnChangedLayerVisibility(layer_id id, bool visible);
  void OnSelectedLayer(layer_id id, const core::ILayer& layer);

  void OnSpawnContextMenu(const QPoint& pos);

  void OnViewChangedSelection(Maybe<QModelIndex> selected, Maybe<QModelIndex>);
  void OnViewChangedName(const QModelIndex& index, const QString& name);

  void OnNewTileLayerRequested();
  void OnNewObjectLayerRequested();

  void OnOpacitySliderValueChanged(int value);

  void OnNewLayerButtonPressed();
  void OnRemoveLayerButtonPressed();

  void OnUpButtonPressed();
  void OnDownButtonPressed();

  void OnDuplicateButtonPressed();

  void OnVisibleButtonToggled(bool visible);
};

}  // namespace tactile
