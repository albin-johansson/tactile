// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/layer.hpp"
#include "core/layer/layer_delegate.hpp"

namespace tactile {

class AbstractLayer : public Layer {
 public:
  ~AbstractLayer() noexcept override = default;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  void set_parent(const Maybe<UUID>& parent_id) override;

  void set_meta_id(int32 id) override;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto get_ctx() -> ContextInfo& override;
  [[nodiscard]] auto get_ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_parent() const -> Maybe<UUID> override;

  [[nodiscard]] auto get_meta_id() const -> Maybe<int32> override;

 protected:
  LayerDelegate mDelegate;
};

}  // namespace tactile
