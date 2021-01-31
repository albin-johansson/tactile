#pragma once

#include "dock_widget.hpp"
#include "map_id.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

namespace tactile::core {
class map_document;
}

namespace tactile::gui {

class tileset_widget;

class tileset_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit tileset_dock(QWidget* parent = nullptr);

 signals:
  void ui_add_tileset();
  void ui_select_tileset(tileset_id id);
  void ui_remove_tileset(tileset_id id);
  void ui_rename_tileset(tileset_id id, const QString& name);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 public slots:
  void added_map(map_id id, const core::map_document& document);

  void switched_map(map_id id);

  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void removed_tileset(tileset_id id);

 private:
  tileset_widget* m_widget{};
};

}  // namespace tactile::gui
