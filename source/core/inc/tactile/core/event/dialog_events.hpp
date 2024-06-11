// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

/**
 * Event for opening the map document creation dialog.
 *
 * \ingroup Event
 */
struct ShowNewMapDialogEvent final
{};

/**
 * Event for opening the map document loading dialog.
 *
 * \ingroup Event
 */
struct ShowOpenMapDialogEvent final
{};

/**
 * Event for opening the 'Save As' dialog.
 *
 * \ingroup Event
 */
struct ShowSaveAsDialogEvent final
{};

/**
 * Event for opening the component editor dialog.
 *
 * \ingroup Event
 */
struct ShowComponentEditorDialogEvent final
{};

/**
 * Event for opening the settings dialog.
 *
 * \ingroup Event
 */
struct ShowSettingsDialogEvent final
{};

/**
 * Event for opening the "About Tactile" dialog.
 *
 * \ingroup Event
 */
struct ShowAboutDialogEvent final
{};

/**
 * Event for opening the "Credits" dialog.
 *
 * \ingroup Event
 */
struct ShowCreditsDialogEvent final
{};

/**
 * Event for opening the dialog for renaming a layer in the active map.
 *
 * \ingroup Event
 */
struct ShowRenameLayerDialogEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

}  // namespace tactile
