// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/document/document.hpp"

namespace tactile {

struct MapSpec;

/**
 * Represents a single map document.
 */
class MapDocument final : public IDocument
{
 public:
  TACTILE_DELETE_COPY(MapDocument);
  TACTILE_DECLARE_MOVE(MapDocument);

  /**
   * Creates a map document.
   *
   * \param spec The map specification to use.
   */
  explicit MapDocument(const MapSpec& spec);

  ~MapDocument() noexcept override;

  void update() override;

  void set_path(Path path) override;

  [[nodiscard]]
  auto get_path() const -> const Path* override;

  [[nodiscard]]
  auto get_registry() -> Registry& override;

  [[nodiscard]]
  auto get_registry() const -> const Registry& override;

  [[nodiscard]]
  auto get_content_size() const -> Float2 override;

  [[nodiscard]]
  auto get_tile_size() const -> Int2 override;

  [[nodiscard]]
  auto get_uuid() const -> const UUID& override;

 private:
  struct Data;
  Unique<Data> mData;
};

}  // namespace tactile
