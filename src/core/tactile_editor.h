#pragma once

#include "smart_pointers.h"

namespace tactile {

class TileMap;

/**
 * The <code>TactileEditor</code> class represents the main interface for the core model of
 * the Tactile application.
 * 
 * @since 0.1.0 
 */
class TactileEditor final {
 public:

  // TODO implement

  /**
   * Creates an instance of the Tactile editor class. There should only be one 
   * instance of this class for every run of the application.
   * 
   * @since 0.1.0 
   */
  TactileEditor();

  ~TactileEditor() noexcept;

  /**
   * Starts working on a new tile map. Any previous unsaved work on another tile map
   * is automatically saved before being closed.
   * 
   * @since 0.1.0
   */
  void new_map() noexcept;

  /**
   * Opens a tile map from the specified location.
   * 
   * @param fileName the file path of the file that holds the tile map, may not be null.
   * @since 0.1.0 
   */
  void open_map(const char* fileName); // TODO support TMX

  void save_as(const char* fileName) const;

  void select_layer(int index) noexcept;

 private:
  UniquePtr<TileMap> m_map;
};

}  // namespace tactile