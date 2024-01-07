// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/document/document.hpp"
#include "tactile/core/document/document_viewport.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/component_set.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Represents a single tilemap document.
 */
class TACTILE_CORE_API MapDocument final : public IDocument {
 public:
  TACTILE_DELETE_COPY(MapDocument);
  TACTILE_DEFAULT_MOVE(MapDocument);

  /**
   * Creates a map document.
   *
   * \param map The associated map.
   */
  explicit MapDocument(Unique<IMap> map);

  ~MapDocument() noexcept override = default;

  void update() override;

  void set_path(FilePath path) override;

  [[nodiscard]]
  auto path() const -> const FilePath* override;

  [[nodiscard]]
  auto component_set() -> ComponentSet& override;

  [[nodiscard]]
  auto component_set() const -> const ComponentSet& override;

  [[nodiscard]]
  auto viewport() -> DocumentViewport& override;

  [[nodiscard]]
  auto viewport() const -> const DocumentViewport& override;

  [[nodiscard]]
  auto find_context(const UUID& uuid) -> IMetaContext* override;

  [[nodiscard]]
  auto find_context(const UUID& uuid) const -> const IMetaContext* override;

  [[nodiscard]]
  auto map() -> IMap&;

  [[nodiscard]]
  auto map() const -> const IMap&;

 private:
  Unique<IMap> mMap;
  DocumentViewport mViewport {};
  ComponentSet mComponentSet {};
  Maybe<FilePath> mPath {};
};

}  // namespace tactile
