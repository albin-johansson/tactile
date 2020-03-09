#include "tactile_editor.h"

#include <QPainter>
#include <iostream>

#include "tile_map.h"

namespace tactile {

TactileEditor::TactileEditor() : m_map{std::make_unique<TileMap>(5, 5)}
{}

TactileEditor::~TactileEditor() noexcept = default;

void TactileEditor::new_map() noexcept
{}

void TactileEditor::open_map(const char*)
{
  // TODO parse Tactile or TMX formats
}

void TactileEditor::save_as(const char*) const
{}

void TactileEditor::add_tile_sheet(const char* fileName) noexcept
{
  if (!fileName) {
    return;
  }

  std::cout << "Adding tile sheet: " << fileName << "\n";
}

void TactileEditor::set_rows(int nRows) noexcept
{
  m_map->set_rows(nRows);
}

void TactileEditor::set_cols(int nCols) noexcept
{
  m_map->set_cols(nCols);
}

int TactileEditor::rows() const noexcept
{
  return m_map->rows();
}

int TactileEditor::cols() const noexcept
{
  return m_map->cols();
}

void TactileEditor::select_layer(int index) noexcept
{
  m_map->select(index);
  emit updated();
}

void TactileEditor::draw(QPainter& painter) const noexcept
{
  m_map->draw(painter);
}

void TactileEditor::add_row() noexcept
{
  m_map->add_row();
  emit updated();
}

void TactileEditor::add_col() noexcept
{
  m_map->add_col();
  emit updated();
}

void TactileEditor::remove_row() noexcept
{
  m_map->remove_row();
  emit updated();
}

void TactileEditor::remove_col() noexcept
{
  m_map->remove_col();
  emit updated();
}

}  // namespace tactile
