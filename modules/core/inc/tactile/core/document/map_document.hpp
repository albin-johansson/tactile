// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/document/document.hpp"
#include "tactile/core/document/document_viewport.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a single tilemap document.
 */
class TACTILE_CORE_API MapDocument final : public IDocument {
 public:
  TACTILE_DELETE_COPY(MapDocument);
  TACTILE_DEFAULT_MOVE(MapDocument);

  /**
   * \brief Creates a map document.
   *
   * \param map the associated map.
   */
  explicit MapDocument(Unique<IMap> map);

  ~MapDocument() noexcept override = default;

  void update() override;

  void set_path(FilePath path) override;

  [[nodiscard]]
  auto path() const -> const FilePath* override;

  [[nodiscard]]
  auto viewport() -> DocumentViewport& override;

  [[nodiscard]]
  auto viewport() const -> const DocumentViewport& override;

  [[nodiscard]]
  auto map() -> IMap&;

  [[nodiscard]]
  auto map() const -> const IMap&;

 private:
  Unique<IMap> mMap;
  DocumentViewport mViewport {};
  Maybe<FilePath> mPath {};
};

}  // namespace tactile
