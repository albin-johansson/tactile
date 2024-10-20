// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

/**
 * A dialog for creating new tilesets.
 */
class NewTilesetDialog final
{
 public:
  /**
   * Pushes the dialog to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The associated event dispatcher.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

  /**
   * Schedules the dialog to be opened.
   */
  void open();

 private:
  std::string mTexturePathPreview {};
  std::filesystem::path mTexturePath {};
  Int2 mTileSize {};
  bool mShouldOpen {false};
};

}  // namespace ui
}  // namespace tactile::core
