#pragma once

#include "dialog.hpp"

namespace tactile {

/**
 * \brief Represents the dialog used to create new tilemaps.
 *
 * \ingroup gui
 */
class create_map_dialog final : public dialog_base
{
 public:
  create_map_dialog();

  void show();

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const Model& model) const -> bool override;

 private:
  int mTileWidth{};
  int mTileHeight{};
  int mRows{};
  int mColumns{};
};

}  // namespace tactile
