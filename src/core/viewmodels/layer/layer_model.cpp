#include "layer_model.hpp"

#include "layer_item.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

layer_model::layer_model(not_null<core::MapDocument*> document)
    : QStandardItemModel{}
    , m_document{document}
{
  if (!m_document)
  {
    throw tactile_error{"layer_model requires non-null map document!"};
  }

  document->EachLayer(
      [this](const layer_id id, const shared<core::ILayer>& layer) {
        Q_ASSERT(layer);
        appendRow(layer_item::make(id, *layer.get()));
      });

  // clang-format off
  connect(m_document, &core::MapDocument::S_AddedLayer,
          this, &layer_model::add_item);

  connect(m_document, &core::MapDocument::S_AddedDuplicatedLayer,
          this, &layer_model::add_item);

  connect(m_document, &core::MapDocument::S_RemovedLayer,
          this, &layer_model::remove_item);

  connect(m_document, &core::MapDocument::S_ChangedLayerOpacity,
          this, &layer_model::changed_opacity);

  connect(m_document, &core::MapDocument::S_ChangedLayerName,
          this, &layer_model::changed_name);

  connect(m_document, &core::MapDocument::S_ChangedLayerVisibility,
          this, &layer_model::changed_visibility);

  connect(m_document, &core::MapDocument::S_MovedLayerForward,
          this, &layer_model::moved_layer_forward);

  connect(m_document, &core::MapDocument::S_MovedLayerBack,
          this, &layer_model::moved_layer_back);

  connect(m_document, &core::MapDocument::S_SelectedLayer,
          this, &layer_model::selected_layer);
  // clang-format on
}

void layer_model::add_tile_layer()
{
  m_document->AddTileLayer();
}

void layer_model::add_object_layer()
{
  m_document->AddObjectLayer();
}

void layer_model::show_properties(const QModelIndex& index)
{
  emit m_document->S_ShowLayerProperties(id_from_index(index));
}

void layer_model::select(const QModelIndex& index)
{
  m_document->SelectLayer(id_from_index(index));
}

void layer_model::remove(const QModelIndex& index)
{
  m_document->RemoveLayer(id_from_index(index));
}

void layer_model::duplicate(const QModelIndex& index)
{
  m_duplicateTargetRow = itemFromIndex(index)->row() + 1;
  m_document->DuplicateLayer(id_from_index(index));
}

void layer_model::move_up(const QModelIndex& index)
{
  m_document->MoveLayerForward(id_from_index(index));
}

void layer_model::move_down(const QModelIndex& index)
{
  m_document->MoveLayerBack(id_from_index(index));
}

void layer_model::set_name(const QModelIndex& index, const QString& name)
{
  m_document->SetLayerName(id_from_index(index), name);
}

void layer_model::set_opacity(const QModelIndex& index, const double opacity)
{
  m_document->SetLayerOpacity(id_from_index(index), opacity);
}

void layer_model::set_visible(const QModelIndex& index, const bool visible)
{
  m_document->SetLayerVisibility(id_from_index(index), visible);
}

auto layer_model::opacity(const QModelIndex& index) const -> double
{
  return m_document->GetLayer(id_from_index(index))->Opacity();
}

auto layer_model::visible(const QModelIndex& index) const -> bool
{
  return m_document->GetLayer(id_from_index(index))->IsVisible();
}

void layer_model::add_item(const layer_id id, const core::ILayer& layer)
{
  if (m_duplicateTargetRow)
  {
    insertRow(*m_duplicateTargetRow, layer_item::make(id, layer));
    m_duplicateTargetRow.reset();
  }
  else
  {
    appendRow(layer_item::make(id, layer));
  }
}

void layer_model::remove_item(const layer_id id)
{
  const auto* root = invisibleRootItem();
  const auto nRows = rowCount();
  for (auto row = 0; row < nRows; ++row)
  {
    if (const auto* item = dynamic_cast<const layer_item*>(root->child(row)))
    {
      if (item->get_id() == id)
      {
        removeRow(item->row());
      }
    }
  }
}

auto layer_model::move_up_in_gui(const layer_id id) -> maybe<QModelIndex>
{
  if (const auto index = index_of(id))
  {
    const auto row = index->row();

    const auto newRow = row - 1;
    Q_ASSERT(newRow != -1);

    insertRow(newRow, takeRow(row));
    return item(newRow)->index();
  }
  else
  {
    return std::nullopt;
  }
}

auto layer_model::move_down_in_gui(const layer_id id) -> maybe<QModelIndex>
{
  if (const auto index = index_of(id))
  {
    const auto row = index->row();

    const auto newRow = row + 1;
    Q_ASSERT(newRow != rowCount());

    insertRow(newRow, takeRow(row));
    return item(newRow)->index();
  }
  else
  {
    return std::nullopt;
  }
}

auto layer_model::get_item(const QModelIndex& index) const -> const layer_item*
{
  return dynamic_cast<const layer_item*>(itemFromIndex(index));
}

auto layer_model::index_of(const layer_id id) const -> maybe<QModelIndex>
{
  const auto topLevelRows = rowCount();
  const auto* root = invisibleRootItem();

  for (auto row = 0; row < topLevelRows; ++row)
  {
    if (const auto* item = dynamic_cast<const layer_item*>(root->child(row, 0)))
    {
      if (id == item->get_id())
      {
        return item->index();
      }
    }
  }

  return std::nullopt;
}

auto layer_model::id_from_index(const QModelIndex& index) const -> layer_id
{
  const auto* item = get_item(index);
  Q_ASSERT(item);
  return item->get_id();
}

void layer_model::moved_layer_forward(const layer_id id)
{
  move_up_in_gui(id);
}

void layer_model::moved_layer_back(const layer_id id)
{
  move_down_in_gui(id);
}

}  // namespace tactile::vm
