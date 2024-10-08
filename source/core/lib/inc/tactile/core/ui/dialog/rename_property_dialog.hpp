// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {

/**
 * A dialog for renaming existing properties.
 */
class RenamePropertyDialog final
{
 public:
  RenamePropertyDialog();

  /**
   * Pushes the dialog to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher that will be used.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

  /**
   * Schedules the dialog to be opened.
   *
   * \param context_entity   The associated meta context.
   * \param target_prop_name The target property in the context.
   */
  void open(EntityID context_entity, std::string target_prop_name);

 private:
  EntityID mContextEntity {kInvalidEntity};
  std::string mTargetPropName {};
  std::string mNewPropName {};
  bool mShouldOpen {false};
};

}  // namespace ui
}  // namespace tactile
