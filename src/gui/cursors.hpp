#pragma once

#include <centurion.hpp>  // system_cursor

namespace tactile {

void LoadCursors();

void UnloadCursors() noexcept;

void EnableCursor(cen::system_cursor cursor);

}  // namespace tactile
