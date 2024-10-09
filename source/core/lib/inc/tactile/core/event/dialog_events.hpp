// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

/**
 * Event for opening the map document creation dialog.
 */
struct ShowNewMapDialogEvent final
{};

/**
 * Event for opening the map document loading dialog.
 */
struct ShowOpenMapDialogEvent final
{};

/**
 * Event for opening the 'Save As' dialog.
 */
struct ShowSaveAsDialogEvent final
{};

/**
 * Event for opening the settings dialog.
 */
struct ShowSettingsDialogEvent final
{};

/**
 * Event for opening the "About Tactile" dialog.
 */
struct ShowAboutDialogEvent final
{};

/**
 * Event for opening the "Credits" dialog.
 */
struct ShowCreditsDialogEvent final
{};

/**
 * Event for opening the dialog for renaming a layer in the active map.
 */
struct ShowRenameLayerDialogEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

}  // namespace tactile::core
