#pragma once

#include <QFileDialog>  // QFileDialog

namespace tactile {

/**
 * \class TilesetImageFileDialog
 *
 * \brief Represents the file dialog used to locate tileset images.
 *
 * \since 0.2.0
 *
 * \headerfile tileset_image_file_dialog.hpp
 */
class TilesetImageFileDialog final : public QFileDialog
{
 public:
  /**
   * \brief Creates a tileset image file dialog.
   *
   * \details The default directory is the standard location for pictures on the
   * current system.
   *
   * \param parent a pointer to the parent widget.
   *
   * \since 0.2.0
   */
  explicit TilesetImageFileDialog(QWidget* parent = nullptr);
};

}  // namespace tactile
