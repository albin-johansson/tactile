// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {

/**
 * A dialog for creating new properties.
 */
class NewPropertyDialog final
{
 public:
  NewPropertyDialog();

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
   * \param context_entity The target meta context.
   */
  void open(EntityID context_entity);

 private:
  EntityID mContextEntity {kInvalidEntity};
  String mName {};
  Attribute mValue {};
  bool mShouldOpen {false};
};

}  // namespace ui
}  // namespace tactile
