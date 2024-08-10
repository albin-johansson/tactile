// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format_id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/map/map_spec.hpp"

namespace tactile {

/**
 * Event for creating new map documents.
 */
struct CreateMapEvent final
{
  /** The requested map specification. */
  MapSpec spec;
};

/**
 * Event for marking the active map as the selected meta context.
 */
struct InspectMapEvent final
{};

/**
 * Event for opening the tileset import dialog.
 */
struct ShowAddTilesetDialogEvent final
{};

/**
 * Event for opening map resize dialog.
 */
struct ShowResizeMapDialogEvent final
{};

/**
 * Event for adding a tile row to the active map.
 */
struct AddRowToMapEvent final
{};

/**
 * Event for adding a tile column to the active map.
 */
struct AddColumnToMapEvent final
{};

/**
 * Event for removing a tile row from the active map.
 */
struct RemoveRowToMapEvent final
{};

/**
 * Event for removing a tile column from the active map.
 */
struct RemoveColumnToMapEvent final
{};

/**
 * Event for fixing all invalid tile identifiers in the active map.
 */
struct FixMapTilesEvent final
{};

/**
 * Event for opening the dialog for exporting the active map using a specific save format.
 */
struct ShowExportMapDialogEvent final
{
  SaveFormatId format_id;
};

}  // namespace tactile
