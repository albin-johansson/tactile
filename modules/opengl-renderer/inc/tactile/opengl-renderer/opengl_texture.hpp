// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/texture.hpp"
#include "tactile/opengl-renderer/api.hpp"

namespace tactile::opengl {

class TACTILE_OPENGL_API OpenGLTexture final : public ITexture {
 public:
  using id_type = uint;

  [[nodiscard]]
  static auto load(const FilePath& image_path) -> Result<OpenGLTexture>;

  OpenGLTexture() = delete;

  ~OpenGLTexture() noexcept override;

  OpenGLTexture(OpenGLTexture&& other) noexcept;

  auto operator=(OpenGLTexture&& other) noexcept -> OpenGLTexture&;

  TACTILE_DELETE_COPY(OpenGLTexture);

  [[nodiscard]]
  auto get_handle() const -> void* override;

  [[nodiscard]]
  auto get_size() const -> Int2 override;

  [[nodiscard]]
  auto get_path() const -> const FilePath& override;

 private:
  id_type mID;
  Int2 mSize;
  FilePath mPath;

  OpenGLTexture(id_type id, Int2 size, FilePath path);

  void _dispose() noexcept;
};

}  // namespace tactile::opengl
