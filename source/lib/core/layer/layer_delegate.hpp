// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/context/context_info.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Used to implement the layer interface.
/// This class does intentionally not inherit from the layer interface.
/// Refer to the layer interface for more documentation on the member functions.
class LayerDelegate final {
 public:
  void set_opacity(float opacity);

  void set_visible(bool visible);

  void set_parent(const Maybe<UUID>& id);

  void set_meta_id(int32 id);

  [[nodiscard]] auto get_opacity() const -> float;

  [[nodiscard]] auto is_visible() const -> bool;

  [[nodiscard]] auto get_parent() const -> const Maybe<UUID>&;

  [[nodiscard]] auto get_meta_id() const -> const Maybe<int32>&;

  [[nodiscard]] auto get_ctx() -> ContextInfo&;
  [[nodiscard]] auto get_ctx() const -> const ContextInfo&;

  /// Clones the layer delegate, but the meta identifier is not copied.
  [[nodiscard]] auto clone() const -> LayerDelegate;

 private:
  ContextInfo mContext;
  Maybe<UUID> mParentId {};
  Maybe<int32> mMetaId;
  float mOpacity {1.0f};
  bool mVisible : 1 {true};
};

}  // namespace tactile
