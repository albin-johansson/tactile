#pragma once

#include <QObject>  // QObject

#include "forward_declare.hpp"

TACTILE_FORWARD_DECLARE(Ui, window)

namespace tactile::gui {

class window;

class window_connections final : public QObject
{
  Q_OBJECT

 public:
  void init(window* window);

 private:
  window* m_window{};
  Ui::window* m_ui{};

  void init_actions();

  void init_dock_connections();

  void init_map_editor();

  void init_tileset_dock();

  void init_tool_dock();
};

}  // namespace tactile::gui
