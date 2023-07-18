/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tool_delegate.hpp"

#include <spdlog/spdlog.h>

#include "cmd/tool/bucket_fill.hpp"
#include "cmd/tool/stamp_sequence.hpp"
#include "common/fmt/entity_formatter.hpp"
#include "common/fmt/tile_pos_formatter.hpp"
#include "model/documents/command_system.hpp"

namespace tactile {

void on_stamp_sequence(Registry& registry, const StampSequenceEvent& event)
{
  spdlog::trace("[StampSequenceEvent] layer: {}", event.layer);

  sys::try_execute<cmd::StampSequence>(registry,
                                       event.layer,
                                       event.old_state,
                                       event.sequence);
}

void on_flood(Registry& registry, const FloodEvent& event)
{
  spdlog::trace("[FloodEvent] layer: {} origin: {} replacement: {}",
                event.layer,
                event.origin,
                event.replacement);

  sys::try_execute<cmd::BucketFill>(registry,
                                    event.layer,
                                    event.origin,
                                    event.replacement);
}

}  // namespace tactile
