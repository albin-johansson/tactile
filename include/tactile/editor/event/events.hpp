// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <utility>

#include "tactile/editor/event/event_id.hpp"

namespace tactile::editor {

/// Represents all kinds of editor events.
///
/// Every event type must provide a public static data member called "kID" with the value
/// of the associated entry in this enum. See the EventQueue class for details.
enum class EventKind : EventID  // NOLINT(*-enum-size)
{
  kQuit,
};

/// Event indicating that the editor should be shut down.
struct QuitEvent final
{
  constexpr static EventID kID = std::to_underlying(EventKind::kQuit);
};

}  // namespace tactile::editor
