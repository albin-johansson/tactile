
#include "tileset.hpp"

#include <QImage>

#include "tactile_types.hpp"

namespace tactile {

Tileset::Tileset(const Shared<QImage>& image, int tileWidth, int tileHeight)
    : m_sheet{image},
      m_tileWidth{(tileWidth < 1) ? 1 : tileWidth},
      m_tileHeight{(tileHeight < 1) ? 1 : tileHeight},
      m_firstID{1}
{
  if (!image) {
    throw BadArg{"Cannot create tileset from null image!"};
  }

  m_rows = height() / m_tileHeight;
  m_cols = width() / m_tileWidth;
  m_nTiles = m_rows * m_cols;
}

Tileset::Tileset(const Tileset& other) noexcept  // NOLINT
    : m_sheet{other.m_sheet},
      m_tileWidth{other.m_tileWidth},
      m_tileHeight{other.m_tileHeight},
      m_nTiles{other.m_nTiles},
      m_rows{other.m_rows},
      m_cols{other.m_cols},
      m_firstID{other.m_firstID},
      m_selection{other.m_selection}
{}

Tileset::Tileset(Tileset&& other) noexcept
    : m_sheet{std::move(other.m_sheet)},
      m_tileWidth{other.m_tileWidth},
      m_tileHeight{other.m_tileHeight},
      m_nTiles{other.m_nTiles},
      m_rows{other.m_rows},
      m_cols{other.m_cols},
      m_firstID{other.m_firstID},
      m_selection{std::move(other.m_selection)}
{}

Tileset::~Tileset() noexcept = default;

Tileset& Tileset::operator=(const Tileset& other) noexcept
{
  if (this != &other) {
    m_sheet = other.m_sheet;
    m_tileWidth = other.m_tileWidth;
    m_tileHeight = other.m_tileHeight;
    m_firstID = other.m_firstID;
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    m_nTiles = other.m_nTiles;
    m_selection = other.m_selection;
  }
  return *this;
}

Tileset& Tileset::operator=(Tileset&& other) noexcept
{
  if (this != &other) {
    m_sheet = std::move(other.m_sheet);
    m_tileWidth = other.m_tileWidth;
    m_tileHeight = other.m_tileHeight;
    m_firstID = other.m_firstID;
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    m_nTiles = other.m_nTiles;
    m_selection = std::move(other.m_selection);
  }
  return *this;
}

Unique<Tileset> Tileset::unique(const Shared<QImage>& image,
                                int tileWidth,
                                int tileHeight)
{
  return std::make_unique<Tileset>(image, tileWidth, tileHeight);
}

Shared<Tileset> Tileset::shared(const Shared<QImage>& image,
                                int tileWidth,
                                int tileHeight)
{
  return std::make_shared<Tileset>(image, tileWidth, tileHeight);
}

void Tileset::set_first_id(TileID firstID) noexcept
{
  if (firstID > 0) {
    m_firstID = firstID;
  }
}

void Tileset::select(int x, int y) noexcept
{
  const auto tile = tile_at(x, y);
  if (tile != empty) {
    m_selection.emplace(tile);
  }
}

void Tileset::clear_selection() noexcept
{
  m_selection.clear();
}

bool Tileset::contains(TileID id) const noexcept
{
  return id >= first_id() && id <= last_id();
}

TileID Tileset::tile_at(int x, int y) const noexcept
{
  if (x < 0 || y < 0 || x > width() || y > height()) {
    return empty;
  } else {
    const auto row = (y / m_tileHeight);
    const auto col = (x / m_tileWidth);
    const auto index = row * m_cols + col;
    return m_firstID + index;
  }
}

int Tileset::width() const noexcept
{
  return m_sheet->width();
}

int Tileset::height() const noexcept
{
  return m_sheet->height();
}

}  // namespace tactile
