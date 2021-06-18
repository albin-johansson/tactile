#pragma once

#include <QStandardItemModel>  // QStandardItemModel

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)
TACTILE_FORWARD_DECLARE(tactile::core, ILayer)

namespace tactile::vm {

class LayerItem;

class LayerModel final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit LayerModel(NotNull<core::MapDocument*> document);

  void AddTileLayer();

  void AddObjectLayer();

  void ShowProperties(const QModelIndex& index);

  void Select(const QModelIndex& index);

  void Remove(const QModelIndex& index);

  void Duplicate(const QModelIndex& index);

  void MoveUp(const QModelIndex& index);

  void MoveDown(const QModelIndex& index);

  void SetName(const QModelIndex& index, const QString& name);

  void SetOpacity(const QModelIndex& index, double opacity);

  void SetVisible(const QModelIndex& index, bool visible);

  [[nodiscard]] auto Opacity(const QModelIndex& index) const -> double;

  [[nodiscard]] auto IsVisible(const QModelIndex& index) const -> bool;

  [[nodiscard]] auto IndexOf(layer_id id) const -> Maybe<QModelIndex>;

 signals:
  void S_ChangedName(layer_id id, const QString& name);
  void S_ChangedOpacity(layer_id id, double opacity);
  void S_ChangedVisibility(layer_id id, bool visible);
  void S_SelectedLayer(layer_id id, const core::ILayer& layer);

 private:
  core::MapDocument* mDocument{};
  Maybe<int> mDuplicateTargetRow;

  void AddItem(layer_id id, const core::ILayer& layer);

  void RemoveItem(layer_id id);

  auto MoveUpInGui(layer_id id) -> Maybe<QModelIndex>;

  auto MoveDownInGui(layer_id id) -> Maybe<QModelIndex>;

  [[nodiscard]] auto GetItem(const QModelIndex& index) const
      -> const LayerItem*;

  [[nodiscard]] auto IdFromIndex(const QModelIndex& index) const -> layer_id;

 private slots:
  void OnMovedLayerForward(layer_id id);

  void OnMovedLayerBack(layer_id id);
};

}  // namespace tactile::vm
