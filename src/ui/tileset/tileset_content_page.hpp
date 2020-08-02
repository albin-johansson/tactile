#pragma once

#include <QWidget>
#include <memory>

#include "tactile_types.hpp"

namespace Ui {
class TilesetContentPageUI;
}

namespace tactile {

class TilesetTab;
struct TilesetInfo;

class TilesetContentPage final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetContentPage(QWidget* parent = nullptr);

  ~TilesetContentPage() noexcept override;

  void add_tileset(const TilesetInfo& info, const QString& tabName) noexcept;

  void remove_tileset(int id) noexcept;

  [[nodiscard]] bool empty() const noexcept;

 signals:
  void s_requested_tileset();

 private:
  Ui::TilesetContentPageUI* m_ui;
  std::unordered_map<int, std::shared_ptr<TilesetTab>> m_tabs;

  //  [[nodiscard]] bool contains_tileset(int id) const noexcept;
};

}  // namespace tactile
