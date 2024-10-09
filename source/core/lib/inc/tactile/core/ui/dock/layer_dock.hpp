// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/base/util/buffer.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class MapDocument;
class EventDispatcher;
class Registry;
struct CMap;
struct CGroupLayer;

namespace ui {

class Language;

/**
 * A dock widget that displays the layers present in a map document.
 */
class LayerDock final
{
 public:
  /**
   * Pushes the layer dock to the widget stack.
   *
   * \param language   The current language.
   * \param document   The currently active map document.
   * \param dispatcher The event dispatcher to use.
   */
  void update(const Language& language,
              const MapDocument& document,
              EventDispatcher& dispatcher);

 private:
  using NameBuffer = Buffer<char, 128>;

  constexpr static const char* kNewLayerPopupId = "##NewLayerPopup";

  void _push_side_button_group(const Registry& registry,
                               const CMap& map,
                               EventDispatcher& dispatcher);

  void _push_layer_group(const Language& language, const Registry& registry, const CMap& map);

  void _push_layer_list_box(const Language& language,
                            const Registry& registry,
                            const CGroupLayer& root_layer);

  void _push_layer_item(const Language& language,
                        const Registry& registry,
                        EntityID layer_id,
                        bool top_level = false);

  void _push_tile_layer_item(const char* name);

  void _push_object_layer_item(const char* name);

  void _push_new_layer_popup(const Language& language, EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile::core
