#pragma once

#include <entt/entt.hpp>

#include "dialog.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Used to change the dimensions of a tilemap.
 */
class resize_map_dialog final : public dialog_base
{
 public:
  TACTILE_DEFAULT_COPY(resize_map_dialog)
  TACTILE_DEFAULT_MOVE(resize_map_dialog)

  resize_map_dialog();

  ~resize_map_dialog() override = default;

  void show(usize nCurrentRows, usize nCurrentColumns);

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  usize mRows{};
  usize mColumns{};
};

}  // namespace tactile
