#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(LayerWidget)
TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, layer)
TACTILE_FORWARD_DECLARE(tactile::vm, layer_model)
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
  void OnSwitchedMap(not_null<core::map_document*> document);

 private:
  unique<Ui::LayerWidget> mUi;
  LayerListView* mView{};
  AddLayerContextMenu* mAddLayerMenu{};
  LayerWidgetContextMenu* mWidgetMenu{};
  LayerItemContextMenu* mItemMenu{};
  unique<vm::layer_model> mModel;

  void UpdateActions(const maybe<QModelIndex>& selected);

 private slots:
  void OnChangedLayerOpacity(layer_id id, double opacity);
  void OnChangedLayerName(layer_id id, const QString& name);
  void OnChangedLayerVisibility(layer_id id, bool visible);
  void OnSelectedLayer(layer_id id, const core::layer& layer);

  void OnSpawnContextMenu(const QPoint& pos);

  void OnViewChangedSelection(maybe<QModelIndex> selected, maybe<QModelIndex>);
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
