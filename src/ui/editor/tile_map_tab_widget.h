#pragma once
#include <QTabWidget>

namespace tactile {

class EditorTab;

class TileMapTabWidget final : public QTabWidget {
  Q_OBJECT

 public:
  explicit TileMapTabWidget(QWidget* parent = nullptr);

  ~TileMapTabWidget() noexcept override;

  void add_tile_map_tab(const QString& title) noexcept;

  void center_viewport(int mapWidth, int mapHeight) noexcept;

 signals:
  void tmtw_req_redraw(QPainter& painter);

 private:
  [[nodiscard]] EditorTab* get_pane(int index) const noexcept;
};

}  // namespace tactile
