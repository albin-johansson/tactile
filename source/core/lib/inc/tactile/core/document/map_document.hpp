// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <memory>        // unique_ptr
#include <system_error>  // error_code

#include "tactile/base/document/document.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

struct MapSpec;
class IRenderer;

namespace ir {
struct Map;
}  // namespace ir

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
   *
   * \return
   * A map document if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto make(const MapSpec& spec) -> std::expected<MapDocument, std::error_code>;

  /**
   * Creates a map document from an intermediate representation.
   *
   * \param renderer The renderer used to load textures.
   * \param ir_map   The intermediate map representation.
   *
   * \return
   * A map document if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto make(IRenderer& renderer,
                   const ir::Map& ir_map) -> std::expected<MapDocument, std::error_code>;

  ~MapDocument() noexcept override;

  [[nodiscard]]
  auto accept(IDocumentVisitor& visitor) const
      -> std::expected<void, std::error_code> override;

  void update() override;

  void set_path(std::filesystem::path path) override;

  void set_format(SaveFormatId format_id) override;

  [[nodiscard]]
  auto get_path() const -> const std::filesystem::path* override;

  [[nodiscard]]
  auto get_format() const -> SaveFormatId override;

  [[nodiscard]]
  auto get_registry() -> Registry& override;

  [[nodiscard]]
  auto get_registry() const -> const Registry& override;

  [[nodiscard]]
  auto get_content_size() const -> Float2 override;

  [[nodiscard]]
  auto get_tile_size() const -> Int2 override;

  [[nodiscard]]
  auto get_extent() const -> Extent2D override;

  [[nodiscard]]
  auto get_uuid() const -> const UUID& override;

 private:
  struct Data;
  std::unique_ptr<Data> mData;

  MapDocument();
};

}  // namespace tactile
