#pragma once

#include <rune/everything.hpp>  // vector_map

#include "abstract_layer.hpp"
#include "aliases/layer_id.hpp"
#include "aliases/layer_map.hpp"
#include "aliases/shared.hpp"

namespace Tactile {

/**
 * \brief Represents a collection of layers.
 *
 * \ingroup core
 *
 * \todo Something like "DragLayerCmd" to support moving layers to/from groups.
 */
class GroupLayer final : public ALayer
{
 public:
  using storage_type = layer_map;
  using const_iterator = storage_type::const_iterator;

  GroupLayer();

  [[nodiscard]] auto Clone() const -> SharedLayer override;

  void AddLayer(layer_id id, SharedLayer layer);

  [[nodiscard]] auto GetLayer(layer_id id) const -> SharedLayer;

  [[nodiscard]] auto FindLayer(layer_id id) -> ILayer*;

  [[nodiscard]] auto FindLayer(layer_id id) const -> const ILayer*;

  [[nodiscard]] auto ContainsLayer(layer_id id) const -> bool;

  [[nodiscard]] auto GetLayers() -> storage_type&;

  [[nodiscard]] auto GetLayers() const -> const storage_type&;

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mLayers.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mLayers.end();
  }

 private:
  storage_type mLayers;
};

}  // namespace Tactile
