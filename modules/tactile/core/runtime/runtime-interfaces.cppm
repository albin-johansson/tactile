// Copyright (C) 2025 Albin Johansson

export module tactile.core.runtime:interfaces;

import tactile.core.prelude;
import tactile.core.error;

export namespace tactile {

/// Type used for texture identifiers.
using TextureID = i32;

/// Interface for a texture loaded by a backend.
class ITexture
{
 protected:
  ITexture() = default;

  ITexture(ITexture&&) noexcept = default;

  ITexture(const ITexture&) = default;

  auto operator=(ITexture&&) noexcept -> ITexture& = default;

  auto operator=(const ITexture&) -> ITexture& = default;

 public:
  virtual ~ITexture() noexcept = default;

  /// Returns the identifier associated with the texture.
  [[nodiscard]]
  virtual auto id() const -> TextureID = 0;
};

/// Interface for a texture manager associated with a backend.
class ITextureManager
{
 protected:
  ITextureManager() = default;

  ITextureManager(ITextureManager&&) noexcept = default;

  ITextureManager(const ITextureManager&) = default;

  auto operator=(ITextureManager&&) noexcept -> ITextureManager& = default;

  auto operator=(const ITextureManager&) -> ITextureManager& = default;

 public:
  virtual ~ITextureManager() noexcept = default;

  /// Loads a texture from the filesystem.
  [[nodiscard]]
  virtual auto load_texture(const Path& path) -> Result<TextureID> = 0;

  /// Removes a previously loaded texture.
  virtual void erase_texture(TextureID id) = 0;

  /// Returns the texture associated with a given identifier, if any.
  [[nodiscard]]
  virtual auto find_texture(TextureID id) const -> const ITexture* = 0;
};

/// Interface for applications usable with backend implementations (see
/// IBackend).
class IApp
{
 protected:
  IApp() = default;

  IApp(IApp&&) noexcept = default;

  IApp(const IApp&) = default;

  auto operator=(IApp&&) noexcept -> IApp& = default;

  auto operator=(const IApp&) -> IApp& = default;

 public:
  virtual ~IApp() noexcept = default;

  /// Called once per event loop iteration, immediately after polling OS events.
  [[nodiscard]]
  virtual auto on_update() -> Result<void> = 0;

  /// Renders UI elements.
  ///
  /// This is the only function in this interface that is allowed to call into
  /// Dear ImGui APIs. Backends should aim to follow each call to on_update with
  /// a call to this function. However, there's no guarantee that this is the
  /// case. In other words, there may be several calls to on_update for every
  /// on_render call.
  virtual void on_render() const = 0;

  /// Called when the application is starting up, before the first on_update
  /// call.
  ///
  /// It's safe for an application to store the passed pointer.
  [[nodiscard]]
  virtual auto on_startup(ITextureManager* texture_manager) -> Result<void> = 0;

  /// Called when the application is shutting down, after the last on_update
  /// call.
  [[nodiscard]]
  virtual auto on_shutdown() -> Result<void> = 0;

  /// Called once per event loop iteration to check if the application wants to
  /// stop.
  [[nodiscard]]
  virtual auto should_stop() const -> bool = 0;
};

/// Interface for backend implementations.
class IBackend
{
 protected:
  IBackend() = default;

  IBackend(IBackend&&) noexcept = default;

  IBackend(const IBackend&) = default;

  auto operator=(IBackend&&) noexcept -> IBackend& = default;

  auto operator=(const IBackend&) -> IBackend& = default;

 public:
  virtual ~IBackend() noexcept = default;

  /// Runs the given application.
  [[nodiscard]]
  virtual auto run(IApp& app) -> Result<void> = 0;
};

}  // namespace tactile
