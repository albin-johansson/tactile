#include "tactile_editor.h"

#include <QImage>

#include "tile_map.h"
#include "tile_sheet.h"
#include "tile_sheet_manager.h"

namespace tactile {

TactileEditor::TactileEditor()
    : m_activeMapIndex{nothing},
      m_sheetManager{std::make_unique<TileSheetManager>()}
{}

TactileEditor::~TactileEditor() noexcept = default;

void TactileEditor::new_map(int id) noexcept
{
  if (m_maps.count(id)) {
    qWarning("Editor core already had tile map associated with %i!", id);
  } else {
    m_maps.insert({id, TileMap::unique(5, 5)});
    m_activeMapIndex = id;
  }
}

// TODO tile maps need to store their current tile size

void TactileEditor::close_map(int id) noexcept
{
  if (m_maps.count(id)) {
    m_maps.erase(id);

    if (m_activeMapIndex == id) {
      m_activeMapIndex = nothing;
    }

    if (!m_maps.empty()) {
      const auto begin = m_maps.begin();
      m_activeMapIndex = begin->first;
    }
  } else {
    qWarning("Attempted to remove tile map that doesn't exist: %i!", id);
  }
}

void TactileEditor::open_map(const char*)
{
  // TODO parse Tactile or TMX formats
  qWarning("\"Open map\" isn't implemented!");
}

void TactileEditor::save_as(const char*) const
{
  qWarning("\"Save as\" isn't implemented!");
}

Maybe<int> TactileEditor::add_tile_sheet(const SharedPtr<QImage>& image,
                                         int tileWidth,
                                         int tileHeight) noexcept
{
  if (!image->isNull()) {
    // TODO add tile width and tile height to tile sheet
    return m_sheetManager->add(TileSheet::unique(image, tileWidth));
  } else {
    return nothing;
  }
}

void TactileEditor::set_rows(int nRows) noexcept
{
  if (auto* map = active_map(); map) {
    map->set_rows(nRows);
  }
}

void TactileEditor::set_cols(int nCols) noexcept
{
  if (auto* map = active_map(); map) {
    map->set_cols(nCols);
  }
}

Maybe<int> TactileEditor::rows() const noexcept
{
  return maybe_get<int>([](const TileMap& map) noexcept { return map.rows(); });
}

Maybe<int> TactileEditor::cols() const noexcept
{
  return maybe_get<int>([](const TileMap& map) noexcept { return map.cols(); });
}

Maybe<int> TactileEditor::map_width() const noexcept
{
  return maybe_get<int>(
      [](const TileMap& map) noexcept { return map.width(); });
}

Maybe<int> TactileEditor::map_height() const noexcept
{
  return maybe_get<int>(
      [](const TileMap& map) noexcept { return map.height(); });
}

Maybe<int> TactileEditor::tile_size() const noexcept
{
  return maybe_get<int>(
      [](const TileMap& map) noexcept { return map.tile_size().size(); });
}

void TactileEditor::select_layer(int index) noexcept
{
  if (auto* map = active_map(); map) {
    map->select(index);
    emit updated();
  }
}

void TactileEditor::select_map(int id) noexcept
{
  if (m_maps.count(id)) {
    m_activeMapIndex = id;
  }
}

void TactileEditor::increase_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->tile_size().increase();
    emit updated();
  }
}

void TactileEditor::decrease_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->tile_size().decrease();
    emit updated();
  }
}

void TactileEditor::reset_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->tile_size().reset();
    emit updated();
  }
}

void TactileEditor::draw(QPainter& painter) const noexcept
{
  if (auto* map = active_map(); map) {
    map->draw(painter);
  }
}

void TactileEditor::add_row() noexcept
{
  if (auto* map = active_map(); map) {
    map->add_row();
    emit updated();
  }
}

void TactileEditor::add_col() noexcept
{
  if (auto* map = active_map(); map) {
    map->add_col();
    emit updated();
  }
}

void TactileEditor::remove_row() noexcept
{
  if (auto* map = active_map(); map) {
    map->remove_row();
    emit updated();
  }
}

void TactileEditor::remove_col() noexcept
{
  if (auto* map = active_map(); map) {
    map->remove_col();
    emit updated();
  }
}

TileMap* TactileEditor::active_map() noexcept
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

const TileMap* TactileEditor::active_map() const noexcept
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

}  // namespace tactile
