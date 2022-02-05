#include "bucket_cmd.hpp"

#include "core/algorithms/flood_fill.hpp"
#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

bucket_cmd::bucket_cmd(registry_ref registry,
                       const tile_position origin,
                       const tile_id replacement)
    : command_base{"Bucket Fill"}
    , mRegistry{registry}
    , mLayer{sys::get_active_layer_id(registry).value()}
    , mOrigin{origin}
    , mReplacement{replacement}
{}

void bucket_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayer);
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::tile_layer>(entity));

  const auto target = mTarget.value();
  auto& layer = registry.get<comp::tile_layer>(entity);
  for (const auto& position : mPositions) {
    layer.matrix.at(position.row_index()).at(position.col_index()) = target;
  }

  mPositions.clear();
  mTarget.reset();
}

void bucket_cmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::get_tile_layer_entity(registry, mLayer);
  const auto& layer = registry.get<comp::tile_layer>(entity);

  mTarget = sys::get_tile(layer, mOrigin);
  flood(registry, entity, mOrigin, mReplacement, mPositions);
}

}  // namespace tactile
