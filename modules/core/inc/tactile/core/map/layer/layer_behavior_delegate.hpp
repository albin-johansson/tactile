// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Used by layer types to implement the common subset of the `ILayer` interface.
 */
class TACTILE_CORE_API LayerBehaviorDelegate final {
 public:
  void set_persistent_id(Maybe<int32> id);

  void set_opacity(float opacity);

  void set_visible(bool visible);

  [[nodiscard]]
  auto get_persistent_id() const -> Maybe<int32>;

  [[nodiscard]]
  auto get_opacity() const -> float;

  [[nodiscard]]
  auto is_visible() const -> bool;

  [[nodiscard]]
  auto meta() -> Metadata&;

  [[nodiscard]]
  auto meta() const -> const Metadata&;

  [[nodiscard]]
  auto clone() const -> LayerBehaviorDelegate;

 private:
  Metadata mMeta;
  Maybe<int32> mPersistentId;
  float mOpacity {1.0f};
  bool mVisible {true};
};

}  // namespace tactile
