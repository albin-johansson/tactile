// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

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

}  // namespace tactile
