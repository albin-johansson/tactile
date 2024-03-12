// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TextureHandle)

/// Represents an OpenGL texture.
class Texture final {
 public:
  TACTILE_DELETE_COPY(Texture);

  Texture(uint id, Int2 size, Path path);

  ~Texture() noexcept;

  Texture(Texture&& other) noexcept;

  auto operator=(Texture&& other) noexcept -> Texture&;

  [[nodiscard]] auto get_id() const -> uint { return mId; }

  [[nodiscard]] auto get_size() const -> Int2 { return mSize; }

  [[nodiscard]] auto get_path() const -> const Path& { return mPath; }

 private:
  uint mId {};
  Int2 mSize {};
  Path mPath;

  void destroy() noexcept;
};

}  // namespace tactile
