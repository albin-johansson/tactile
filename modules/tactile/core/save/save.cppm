// Copyright (C) 2025 Albin Johansson

/// Provides the save format API.
export module tactile.core.save;

import tactile.core.prelude;
import tactile.core.io;
import tactile.core.error;

export namespace tactile {

/// Represents supported save format types.
enum class SaveFormatKind : u8
{
  kTactileYaml,
  kTiledJson,
  kTiledXml,
  kGodotScene,
};

/// Provides options for decoding save files.
struct SaveDecoderOptions final
{};

/// Provides options for encoding save files.
struct SaveEncoderOptions final
{};

/// Interface for save format decoder implementations.
class ISaveDecoder
{
 protected:
  ISaveDecoder() = default;

  ISaveDecoder(ISaveDecoder&&) noexcept = default;

  ISaveDecoder(const ISaveDecoder&) = default;

  auto operator=(ISaveDecoder&&) noexcept -> ISaveDecoder& = default;

  auto operator=(const ISaveDecoder&) -> ISaveDecoder& = default;

 public:
  virtual ~ISaveDecoder() noexcept = default;

  /// Reads a map from a file.
  [[nodiscard]]
  virtual auto decode_map(const Path& path,
                          const Compressor& compressor,
                          const SaveDecoderOptions& options)
      -> Result<void> = 0;
};

/// Interface for save format encoder implementations.
class ISaveEncoder
{
 protected:
  ISaveEncoder() = default;

  ISaveEncoder(ISaveEncoder&&) noexcept = default;

  ISaveEncoder(const ISaveEncoder&) = default;

  auto operator=(ISaveEncoder&&) noexcept -> ISaveEncoder& = default;

  auto operator=(const ISaveEncoder&) -> ISaveEncoder& = default;

 public:
  virtual ~ISaveEncoder() noexcept = default;
};

}  // namespace tactile
