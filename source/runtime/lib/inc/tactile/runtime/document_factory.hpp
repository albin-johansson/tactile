// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr

#include "tactile/base/document/document.hpp"
#include "tactile/base/document/map_view.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/render/renderer.hpp"
#include "tactile/runtime/api.hpp"

namespace tactile::runtime {

/**
 * Creates a map document from an intermediate representation.
 *
 * \note This function is intended for testing use only.
 *
 * \param renderer The associated renderer.
 * \param ir_map   The intermediate map to use as a reference.
 *
 * \return
 * A unique pointer to the created map document, may be null.
 */
[[nodiscard]]
TACTILE_RUNTIME_API auto make_map_document(IRenderer& renderer, const ir::Map& ir_map)
    -> std::unique_ptr<IDocument>;

/**
 * Creates a map view from a map document.
 *
 * \note This function is intended for testing use only.
 *
 * \param document The map document to view.
 *
 * \return
 * A view of the given map document. A null pointer is returned if the document doesn't
 * represent a map.
 */
[[nodiscard]]
TACTILE_RUNTIME_API auto make_map_view(const IDocument& document) -> std::unique_ptr<IMapView>;

}  // namespace tactile::runtime
