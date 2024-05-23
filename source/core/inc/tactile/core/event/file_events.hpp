// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Event for saving the current document to disk.
 *
 * \ingroup Event
 */
struct SaveEvent final
{};

/**
 * Event for saving the current document to a new location.
 *
 * \ingroup Event
 */
struct SaveAsEvent final
{
  /** The chosen file path. */
  Path path;
};

/**
 * Event for closing the current document.
 *
 * \ingroup Event
 */
struct CloseEvent final
{};

/**
 * Event for gracefully shutting down the application.
 *
 * \ingroup Event
 */
struct QuitEvent final
{};

}  // namespace tactile
