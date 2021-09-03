#include "map_system.hpp"

#include <cassert>   // assert
#include <concepts>  // invocable

#include "core/components/layer.hpp"
#include "core/components/tile_layer.hpp"
#include "core/map.hpp"
#include "tile_layer_system.hpp"

namespace Tactile::Sys {
namespace {

template <std::invocable<TileLayer&> T>
void TileLayerQuery(entt::registry& registry, T&& invocable)
{
  for (auto&& [entity, layer] : registry.view<Layer>().each())
  {
    if (auto* tileLayer = registry.try_get<TileLayer>(entity))
    {
      invocable(*tileLayer);
    }
  }
}

}  // namespace

void AddRow(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  ++map.row_count;

  TileLayerQuery(registry, [nCols = map.column_count](TileLayer& layer) {
    layer.matrix.push_back(MakeTileRow(nCols, empty_tile));
  });
}

void AddColumn(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  ++map.column_count;

  TileLayerQuery(registry, [](TileLayer& layer) {
    for (auto& row : layer.matrix)
    {
      row.push_back(empty_tile);
    }
  });
}

void RemoveRow(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  if (map.row_count > 1)
  {
    --map.row_count;
    TileLayerQuery(registry, [](TileLayer& layer) { layer.matrix.pop_back(); });
  }
}

void RemoveColumn(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  if (map.row_count > 1)
  {
    --map.column_count;
    TileLayerQuery(registry, [](TileLayer& layer) {
      for (auto& row : layer.matrix)
      {
        assert(row.size() > 1);
        row.pop_back();
      }
    });
  }
}

}  // namespace Tactile::Sys
