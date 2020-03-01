#pragma once

//#include "image.h"

namespace tactile {

class TileSet final {
 public:
  class Cell;

  void select(int x, int y) noexcept;

  void clear_selection() noexcept;

  [[nodiscard]] int get_tile_amount() const noexcept;

  [[nodiscard]] int width() const noexcept;

  [[nodiscard]] int height() const noexcept;

//  [[nodiscard]] const Image& sheet() const noexcept;

 private:

};

class TileSet::Cell final {
 public:
  Cell(int x, int y, int index) noexcept;

  [[nodiscard]] int x() const noexcept
  {
    return m_x;
  }

  [[nodiscard]] int y() const noexcept
  {
    return m_y;
  }

  [[nodiscard]] int index() const noexcept
  {
    return m_index;
  }

 private:
  int m_x;
  int m_y;
  int m_index;
};

}  // namespace tactile
