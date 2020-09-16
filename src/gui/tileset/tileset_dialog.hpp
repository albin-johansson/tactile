#pragma once

#include <qdialog.h>
#include <qimage.h>
#include <qvalidator.h>

#include <concepts>
#include <optional>

#include "types.hpp"

class QLineEdit;

namespace Ui {
class tileset_dialog;
}

namespace tactile::gui {

/**
 * @interface TilesetDialogCallback
 *
 * @brief Requires that the type is a valid tileset dialog callback.
 *
 * @tparam T the type that will be checked.
 *
 * @since 0.1.0
 */
template <typename T>
concept TilesetDialogCallback = std::invocable<T,
                                               const QImage&,
                                               const QString&,
                                               tile_width,
                                               tile_height,
                                               const QString&>;

/**
 * @class tileset_dialog
 *
 * @brief Represents the dialog used to add new tilesets.
 *
 * @since 0.1.0
 *
 * @headerfile tileset_dialog.hpp
 */
class tileset_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit tileset_dialog(QWidget* parent = nullptr);

  ~tileset_dialog() noexcept override;

  template <TilesetDialogCallback T>
  static void spawn(T&& callback)
  {
    tileset_dialog dialog;
    if (dialog.exec()) {
      const auto& image = dialog.m_image;
      const auto tileWidth = dialog.m_width;
      const auto tileHeight = dialog.m_height;
      const auto name = dialog.m_imageName;
      if (!image.isNull() && tileWidth && tileHeight) {
        callback(image,
                 dialog.m_path,
                 *tileWidth,
                 *tileHeight,
                 name ? *name : "Untitled");
      }
    }
  }

 private:
  owner<Ui::tileset_dialog*> m_ui;
  QImage m_image{};
  QString m_path{};
  std::optional<tile_width> m_width;
  std::optional<tile_height> m_height;
  std::optional<QString> m_imageName;
  QIntValidator* m_validator;
  QPixmap m_defaultImageIcon;

  void validate_input();

  [[nodiscard]] auto is_valid() const -> bool;

  /**
   * Returns a pointer to the "OK" button.
   *
   * @return a pointer to the "OK" button.
   * @since 0.1.0
   */
  [[nodiscard]] auto ok_button() -> QPushButton*;

  /**
   * Indicates whether or not the supplied line edit widget has valid input.
   *
   * @param edit the line edit widget that will be checked.
   * @return true if the line edit widget is valid; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] auto validate(const QLineEdit& edit) const -> QValidator::State;

 private slots:
  void on_imageButton_pressed();

  void on_widthEdit_textChanged();

  void on_heightEdit_textChanged();
};

}  // namespace tactile::gui
