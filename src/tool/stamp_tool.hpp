#pragma once

#include <optional>

#include "abstract_tool.hpp"
#include "tileset.hpp"

namespace tactile {

/**
 * @class stamp_tool
 *
 * @brief Represents a "stamp" tool, which applies the current tileset selection
 * to the tiles in the map.
 *
 * @since 0.1.0
 *
 * @headerfile stamp_tool.hpp
 */
class stamp_tool final : public abstract_tool
{
 public:
  /**
   * @brief Creates an `stamp_tool` instance.
   *
   * @param model a pointer to the associated model, cannot be null.
   *
   * @throws tactile_error if the supplied pointer is null.
   *
   * @since 0.1.0
   */
  explicit stamp_tool(core::model* model);

  ~stamp_tool() noexcept override = default;

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void moved(QMouseEvent* event, const QPointF& mapPosition) override;

  void released(QMouseEvent* event, const QPointF& mapPosition) override;

  void exited(QEvent* event) override;

  void disable() override;

 private:
  /**
   * @brief Applies the current tileset selection to the map.
   *
   * @param position the position of the origin tile that will be changed.
   * @param tileset the currently active tileset.
   *
   * @since 0.1.0
   */
  void apply_current_selection_to_map(const core::position& position,
                                      const core::tileset& tileset);
};

}  // namespace tactile
