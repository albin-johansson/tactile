#include "layer_model.hpp"

#include "layer_item.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

LayerModel::LayerModel(NotNull<core::MapDocument*> document)
    : QStandardItemModel{}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw TactileError{"Layer model requires non-null map document!"};
  }

  document->EachLayer([this](layer_id id, const Shared<core::ILayer>& layer) {
    Q_ASSERT(layer);
    appendRow(new LayerItem{id, *layer.get()});
  });

  // clang-format off
  connect(mDocument, &core::MapDocument::S_AddedLayer, this, &LayerModel::AddItem);
  connect(mDocument, &core::MapDocument::S_AddedDuplicatedLayer, this, &LayerModel::AddItem);
  connect(mDocument, &core::MapDocument::S_RemovedLayer, this, &LayerModel::RemoveItem);
  connect(mDocument, &core::MapDocument::S_ChangedLayerOpacity, this, &LayerModel::S_ChangedOpacity);
  connect(mDocument, &core::MapDocument::S_ChangedLayerName, this, &LayerModel::S_ChangedName);
  connect(mDocument, &core::MapDocument::S_ChangedLayerVisibility, this, &LayerModel::S_ChangedVisibility);
  connect(mDocument, &core::MapDocument::S_SelectedLayer, this, &LayerModel::S_SelectedLayer);
  connect(mDocument, &core::MapDocument::S_MovedLayerForward, this, &LayerModel::OnMovedLayerForward);
  connect(mDocument, &core::MapDocument::S_MovedLayerBack, this, &LayerModel::OnMovedLayerBack);
  // clang-format on
}

void LayerModel::AddTileLayer()
{
  mDocument->AddTileLayer();
}

void LayerModel::AddObjectLayer()
{
  mDocument->AddObjectLayer();
}

void LayerModel::ShowProperties(const QModelIndex& index)
{
  emit mDocument->S_ShowLayerProperties(IdFromIndex(index));
}

void LayerModel::Select(const QModelIndex& index)
{
  mDocument->SelectLayer(IdFromIndex(index));
}

void LayerModel::Remove(const QModelIndex& index)
{
  mDocument->RemoveLayer(IdFromIndex(index));
}

void LayerModel::Duplicate(const QModelIndex& index)
{
  mDuplicateTargetRow = itemFromIndex(index)->row() + 1;
  mDocument->DuplicateLayer(IdFromIndex(index));
}

void LayerModel::MoveUp(const QModelIndex& index)
{
  mDocument->MoveLayerForward(IdFromIndex(index));
}

void LayerModel::MoveDown(const QModelIndex& index)
{
  mDocument->MoveLayerBack(IdFromIndex(index));
}

void LayerModel::SetName(const QModelIndex& index, const QString& name)
{
  mDocument->SetLayerName(IdFromIndex(index), name);
}

void LayerModel::SetOpacity(const QModelIndex& index, double opacity)
{
  mDocument->SetLayerOpacity(IdFromIndex(index), opacity);
}

void LayerModel::SetVisible(const QModelIndex& index, bool visible)
{
  mDocument->SetLayerVisibility(IdFromIndex(index), visible);
}

auto LayerModel::Opacity(const QModelIndex& index) const -> double
{
  return mDocument->GetLayer(IdFromIndex(index))->Opacity();
}

auto LayerModel::IsVisible(const QModelIndex& index) const -> bool
{
  return mDocument->GetLayer(IdFromIndex(index))->IsVisible();
}

void LayerModel::AddItem(const layer_id id, const core::ILayer& layer)
{
  if (mDuplicateTargetRow)
  {
    insertRow(*mDuplicateTargetRow, new LayerItem{id, layer});
    mDuplicateTargetRow.reset();
  }
  else
  {
    appendRow(new LayerItem{id, layer});
  }
}

void LayerModel::RemoveItem(const layer_id id)
{
  const auto* root = invisibleRootItem();
  const auto nRows = rowCount();
  for (auto row = 0; row < nRows; ++row)
  {
    if (const auto* item = dynamic_cast<const LayerItem*>(root->child(row)))
    {
      if (item->GetId() == id)
      {
        removeRow(item->row());
      }
    }
  }
}

auto LayerModel::MoveUpInGui(const layer_id id) -> Maybe<QModelIndex>
{
  if (const auto index = IndexOf(id))
  {
    const auto row = index->row();

    const auto newRow = row - 1;
    Q_ASSERT(newRow != -1);

    insertRow(newRow, takeRow(row));
    return item(newRow)->index();
  }
  else
  {
    return nothing;
  }
}

auto LayerModel::MoveDownInGui(const layer_id id) -> Maybe<QModelIndex>
{
  if (const auto index = IndexOf(id))
  {
    const auto row = index->row();

    const auto newRow = row + 1;
    Q_ASSERT(newRow != rowCount());

    insertRow(newRow, takeRow(row));
    return item(newRow)->index();
  }
  else
  {
    return nothing;
  }
}

auto LayerModel::GetItem(const QModelIndex& index) const -> const LayerItem*
{
  return dynamic_cast<const LayerItem*>(itemFromIndex(index));
}

auto LayerModel::IndexOf(const layer_id id) const -> Maybe<QModelIndex>
{
  const auto topLevelRows = rowCount();
  const auto* root = invisibleRootItem();

  for (auto row = 0; row < topLevelRows; ++row)
  {
    if (const auto* item = dynamic_cast<const LayerItem*>(root->child(row, 0)))
    {
      if (id == item->GetId())
      {
        return item->index();
      }
    }
  }

  return nothing;
}

auto LayerModel::IdFromIndex(const QModelIndex& index) const -> layer_id
{
  const auto* item = GetItem(index);
  Q_ASSERT(item);
  return item->GetId();
}

void LayerModel::OnMovedLayerForward(const layer_id id)
{
  MoveUpInGui(id);
}

void LayerModel::OnMovedLayerBack(const layer_id id)
{
  MoveDownInGui(id);
}

}  // namespace tactile::vm
