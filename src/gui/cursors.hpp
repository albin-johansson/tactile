#pragma once

#include <centurion.hpp>  // system_cursor

namespace Tactile {

void LoadCursors();

void UnloadCursors() noexcept;

void EnableCursor(cen::system_cursor cursor);

class Cursors final
{
 public:
  Cursors()
  {
    LoadCursors();
  }

  ~Cursors() noexcept
  {
    UnloadCursors();
  }
};

}  // namespace Tactile
