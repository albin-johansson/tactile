#pragma once

#include <QWidget>
#include <memory>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

namespace Ui {
class TilesetContentPageUI;
}

namespace tactile::ui {

class tileset_content_page final : public QWidget {
  Q_OBJECT

 public:
  explicit tileset_content_page(QWidget* parent = nullptr);

  ~tileset_content_page() noexcept override;

  void add_tileset(const model::TilesetInfo& info,
                   const QString& tabName) noexcept;

  void remove_tileset(int id) noexcept;

  [[nodiscard]] auto empty() const noexcept -> bool;

 signals:
  void s_requested_tileset();

 private:
  owner<Ui::TilesetContentPageUI*> m_ui;
  std::unordered_map<int, std::shared_ptr<tileset_tab>> m_tabs;

  //  [[nodiscard]] bool contains_tileset(int id) const noexcept;
};

}  // namespace tactile::ui
