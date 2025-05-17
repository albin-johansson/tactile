// Copyright (C) 2025 Albin Johansson

/// This module provides the global runtime context, accessible via the get_runtime
/// function. The runtime context is used by other Tactile targets to access and
/// configure common resources, such as save format implementations.
///
/// This module is not used by any of the other 'tactile.core' modules.
export module tactile.core.runtime;

export import tactile.core.common;
export import tactile.core.save;
export import tactile.core.io;
export import :interfaces;
export import :null;

export namespace tactile {

/// Represents the Tactile runtime context.
class Runtime final
{
 public:
  /// Adds or removes a save encoder implementation for a given format.
  void set_save_encoder(SaveFormatKind kind, ISaveEncoder* encoder);

  /// Adds or removes a save decoder implementation for a given format.
  void set_save_decoder(SaveFormatKind kind, ISaveDecoder* decoder);

  /// Returns the registered save encoder implementation for a given format.
  [[nodiscard]]
  auto find_save_encoder(SaveFormatKind kind) -> ISaveEncoder*;

  /// Returns the registered save encoder implementation for a given format.
  [[nodiscard]]
  auto find_save_encoder(SaveFormatKind kind) const -> const ISaveEncoder*;

  /// Returns the registered save decoder implementation for a given format.
  [[nodiscard]]
  auto find_save_decoder(SaveFormatKind kind) -> ISaveDecoder*;

  /// Returns the registered save decoder implementation for a given format.
  [[nodiscard]]
  auto find_save_decoder(SaveFormatKind kind) const -> const ISaveDecoder*;

  /// Returns the associated compressor.
  [[nodiscard]]
  auto compressor() -> Compressor&;

  /// Returns the associated compressor.
  [[nodiscard]]
  auto compressor() const -> const Compressor&;

 private:
  HashMap<SaveFormatKind, ISaveEncoder*> m_save_encoders {};
  HashMap<SaveFormatKind, ISaveDecoder*> m_save_decoders {};
  Compressor m_compressor {};
};

/// Returns the global runtime context.
[[nodiscard]]
auto get_runtime() -> Runtime&;

}  // namespace tactile
