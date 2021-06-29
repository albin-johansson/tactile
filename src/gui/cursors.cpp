#include "cursors.hpp"

#include <unordered_map>  // unordered_map

namespace Tactile {
namespace {

inline std::unordered_map<cen::system_cursor, cen::cursor> cursors;

}  // namespace

void LoadCursors()
{
  using sys_cursor = cen::system_cursor;

  cursors.try_emplace(sys_cursor::arrow, sys_cursor::arrow);
  cursors.try_emplace(sys_cursor::ibeam, sys_cursor::ibeam);
  cursors.try_emplace(sys_cursor::wait, sys_cursor::wait);
  cursors.try_emplace(sys_cursor::crosshair, sys_cursor::crosshair);
  cursors.try_emplace(sys_cursor::wait_arrow, sys_cursor::wait_arrow);
  cursors.try_emplace(sys_cursor::arrow_nw_se, sys_cursor::arrow_nw_se);
  cursors.try_emplace(sys_cursor::arrow_ne_sw, sys_cursor::arrow_ne_sw);
  cursors.try_emplace(sys_cursor::arrow_w_e, sys_cursor::arrow_w_e);
  cursors.try_emplace(sys_cursor::arrow_n_s, sys_cursor::arrow_n_s);
  cursors.try_emplace(sys_cursor::arrow_all, sys_cursor::arrow_all);
  cursors.try_emplace(sys_cursor::no, sys_cursor::no);
  cursors.try_emplace(sys_cursor::hand, sys_cursor::hand);
}

void UnloadCursors() noexcept
{
  cursors.clear();
}

void EnableCursor(const cen::system_cursor cursor)
{
  cursors.at(cursor).enable();
}

}  // namespace Tactile
