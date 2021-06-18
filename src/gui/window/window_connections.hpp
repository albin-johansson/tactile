#pragma once

#include <QObject>  // QObject

#include "gui_fwd.hpp"
#include "tactile_declare_ui.hpp"

TACTILE_DECLARE_UI(Window)

namespace tactile {

class WindowConnections final : public QObject
{
  Q_OBJECT

 public:
  void Init(Window* window);

 private:
  Window* mWindow{};
  Ui::Window* mUi{};

  void InitActions();

  void InitDockConnections();

  void InitMapEditor();

  void InitTilesetDock();

  void InitToolDock();
};

}  // namespace tactile
