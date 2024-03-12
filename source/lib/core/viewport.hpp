// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

struct ViewportLimits final {
  Float2 min_offset {};
  Float2 max_offset {};
};

class Viewport final {
 public:
  void reset_limits();

  void offset(const Float2& delta);

  void pan_left();

  void pan_right();

  void pan_up();

  void pan_down();

  void zoom_in(const Float2& anchor);

  void zoom_out(const Float2& anchor);

  void set_tile_size(const Float2& size);

  void set_limits(const ViewportLimits& limits);

  [[nodiscard]] auto can_zoom_out() const -> bool;

  [[nodiscard]] auto scaling_ratio(const Float2& tile_size) const -> Float2;

  [[nodiscard]] auto get_offset() const noexcept -> const Float2& { return mOffset; }

  [[nodiscard]] auto tile_size() const noexcept -> const Float2& { return mTileSize; }

  [[nodiscard]] auto limits() const noexcept -> const Maybe<ViewportLimits>&
  {
    return mLimits;
  }

 private:
  Float2 mOffset {};
  Float2 mTileSize {32, 32};
  Maybe<ViewportLimits> mLimits;
};

}  // namespace tactile
