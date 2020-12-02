#pragma once

#include <QFileDialog>

namespace tactile::gui {

/**
 * \class tileset_image_file_dialog
 *
 * \brief Represents the file dialog used to locate tileset images.
 *
 * \since 0.2.0
 *
 * \headerfile tileset_image_file_dialog.hpp
 */
class tileset_image_file_dialog final : public QFileDialog
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
  explicit tileset_image_file_dialog(QWidget* parent = nullptr);
};

}  // namespace tactile::gui
