// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/layer_view_impl.hpp"

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/document/object_view_impl.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

LayerViewImpl::LayerViewImpl(const MapDocument* document,
                             const ILayerView* parent_layer,
                             const EntityID layer_id)
  : mDocument {require_not_null(document, "null document")},
    mParentLayer {parent_layer},
    mLayerId {layer_id},
    mMeta {document, mLayerId}
{}

void LayerViewImpl::accept(IDocumentVisitor& visitor) const
{
  const auto& registry = mDocument->get_registry();

  visitor.visit(*this);

  if (is_group_layer(registry, mLayerId)) {
    const auto& group_layer = registry.get<CGroupLayer>(mLayerId);

    for (const auto sublayer_id : group_layer.layers) {
      const LayerViewImpl sublayer_view {mDocument, this, sublayer_id};
      sublayer_view.accept(visitor);
    }
  }
  else if (is_object_layer(registry, mLayerId)) {
    const auto& object_layer = registry.get<CObjectLayer>(mLayerId);

    for (const auto object_id : object_layer.objects) {
      const ObjectViewImpl object_view {mDocument, this, object_id};
      object_view.accept(visitor);
    }
  }
}

void LayerViewImpl::write_tile_bytes(ByteStream& byte_stream) const
{
  const auto& registry = mDocument->get_registry();

  if (is_tile_layer(registry, mLayerId)) {
    get_tile_layer_data(registry, mLayerId).write_bytes(byte_stream);
  }
}

auto LayerViewImpl::get_parent_layer() const -> const ILayerView*
{
  return mParentLayer;
}

auto LayerViewImpl::get_id() const -> LayerID
{
  const auto& registry = mDocument->get_registry();
  const auto& layer = registry.get<CLayer>(mLayerId);

  return layer.persistent_id.value();
}

auto LayerViewImpl::get_type() const -> LayerType
{
  const auto& registry = mDocument->get_registry();

  if (is_tile_layer(registry, mLayerId)) {
    return LayerType::kTileLayer;
  }

  if (is_object_layer(registry, mLayerId)) {
    return LayerType::kObjectLayer;
  }

  if (is_group_layer(registry, mLayerId)) {
    return LayerType::kGroupLayer;
  }

  throw Exception {"unexpected layer type"};
}

auto LayerViewImpl::get_opacity() const -> float
{
  const auto& registry = mDocument->get_registry();
  const auto& layer = registry.get<CLayer>(mLayerId);

  return layer.opacity;
}

auto LayerViewImpl::is_visible() const -> bool
{
  const auto& registry = mDocument->get_registry();
  const auto& layer = registry.get<CLayer>(mLayerId);

  return layer.visible;
}

auto LayerViewImpl::get_global_index() const -> usize
{
  const auto& registry = mDocument->get_registry();

  const auto& document_info = registry.get<CDocumentInfo>();
  const auto& map = registry.get<CMap>(document_info.root);

  return get_global_layer_index(registry, map.root_layer, mLayerId).value();
}

auto LayerViewImpl::layer_count() const -> usize
{
  const auto& registry = mDocument->get_registry();

  if (const auto* group = registry.find<CGroupLayer>(mLayerId)) {
    return group->layers.size();
  }

  return 0;
}

auto LayerViewImpl::object_count() const -> usize
{
  const auto& registry = mDocument->get_registry();

  if (const auto* object_layer = registry.find<CObjectLayer>(mLayerId)) {
    return object_layer->objects.size();
  }

  return 0;
}

auto LayerViewImpl::get_tile(const MatrixIndex& index) const -> Optional<TileID>
{
  const auto& registry = mDocument->get_registry();

  if (!is_tile_layer(registry, mLayerId)) {
    return kNone;
  }

  return get_tile_layer_data(registry, mLayerId).at(index);
}

auto LayerViewImpl::get_tile_encoding() const -> TileEncoding
{
  return _get_tile_format().encoding;
}

auto LayerViewImpl::get_tile_compression() const -> Optional<CompressionFormat>
{
  return _get_tile_format().compression;
}

auto LayerViewImpl::get_compression_level() const -> Optional<int>
{
  return _get_tile_format().comp_level;
}

auto LayerViewImpl::get_extent() const -> Optional<MatrixExtent>
{
  const auto& registry = mDocument->get_registry();

  if (!is_tile_layer(registry, mLayerId)) {
    return kNone;
  }

  return get_tile_layer_data(registry, mLayerId).get_extent();
}

auto LayerViewImpl::get_meta() const -> const IMetaView&
{
  return mMeta;
}

auto LayerViewImpl::_get_tile_format() const -> const CTileFormat&
{
  const auto& registry = mDocument->get_registry();
  const auto& document_info = registry.get<CDocumentInfo>();
  return registry.get<CTileFormat>(document_info.root);
}

}  // namespace tactile
