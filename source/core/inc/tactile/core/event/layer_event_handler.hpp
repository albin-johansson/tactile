// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

struct CreateLayerEvent;
struct RemoveLayerEvent;
struct DuplicateLayerEvent;
struct MoveLayerUpEvent;
struct MoveLayerDownEvent;
struct RenameLayerEvent;
struct SetLayerOpacityEvent;
struct SetLayerVisibleEvent;

/**
 * Handles events related to map layers.
 */
class LayerEventHandler final
{
 public:
  /**
   * Creates a layer event handler.
   *
   * \param model The associated model, cannot be null.
   */
  explicit LayerEventHandler(Model* model);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Adds a new layer to the active map.
   *
   * \param event The associated event.
   */
  void on_create_layer(const CreateLayerEvent& event);

  /**
   * Removes a layer from the active map.
   *
   * \param event The associated event.
   */
  void on_remove_layer(const RemoveLayerEvent& event);

  /**
   * Duplicates an existing layer in the active map.
   *
   * \param event The associated event.
   */
  void on_duplicate_layer(const DuplicateLayerEvent& event);

  /**
   * Moves a layer up in the layer stack of the active map.
   *
   * \param event The associated event.
   */
  void on_move_layer_up(const MoveLayerUpEvent& event);

  /**
   * Moves a layer down in the layer stack of the active map.
   *
   * \param event The associated event.
   */
  void on_move_layer_down(const MoveLayerDownEvent& event);

  /**
   * Changes the name of a layer in the active map.
   *
   * \param event The associated event.
   */
  void on_rename_layer(const RenameLayerEvent& event);

  /**
   * Changes the opacity of a layer in the active map.
   *
   * \param event The associated event.
   */
  void on_set_layer_opacity(const SetLayerOpacityEvent& event);

  /**
   * Changes the visibility of a layer in the active map.
   *
   * \param event The associated event.
   */
  void on_set_layer_visible(const SetLayerVisibleEvent& event);

 private:
  Model* mModel;
};

}  // namespace tactile
