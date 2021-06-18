#pragma once

#include <QDialog>     // QDialog
#include <QFileInfo>   // QFileInfo
#include <QImage>      // QImage
#include <QPixmap>     // QPixmap
#include <QString>     // QString
#include <QValidator>  // QValidator
#include <concepts>    // invocable

#include "maybe.hpp"
#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"
#include "tile_height.hpp"
#include "tile_width.hpp"

class QLineEdit;

TACTILE_DECLARE_UI(TilesetDialog)

namespace tactile {

struct TilesetInfo final
{
  QImage image;
  QString name;
  tile_width tileWidth;
  tile_height tileHeight;
  QFileInfo path;
};

/**
 * \class TilesetDialog
 *
 * \brief Represents the dialog used to add new tilesets.
 *
 * \since 0.1.0
 *
 * \headerfile tileset_dialog.hpp
 */
class TilesetDialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit TilesetDialog(QWidget* parent = nullptr);

  ~TilesetDialog() noexcept override;

  template <std::invocable<const TilesetInfo&> T>
  static void Spawn(T&& callback)
  {
    TilesetDialog dialog;
    if (dialog.exec())
    {
      const auto& image = dialog.mImage;
      const auto tileWidth = dialog.mTileWidth;
      const auto tileHeight = dialog.mTileHeight;
      if (!image.isNull() && tileWidth && tileHeight)
      {
        const auto& name = dialog.mImageName;
        const TilesetInfo info{
            .image = dialog.mImage,
            .name = name ? *name : QStringLiteral(u"Untitled"),
            .tileWidth = *tileWidth,
            .tileHeight = *tileHeight,
            .path = QFileInfo{dialog.mPath}};
        callback(info);
      }
    }
  }

 private:
  Unique<Ui::TilesetDialog> mUi;
  QImage mImage;
  QString mPath;
  Maybe<tile_width> mTileWidth;
  Maybe<tile_height> mTileHeight;
  Maybe<QString> mImageName;
  QIntValidator* mValidator;
  QPixmap mDefaultImageIcon;

  void ValidateInput();

  [[nodiscard]] auto IsValid() const -> bool;

  /**
   * Returns a pointer to the "OK" button.
   *
   * \return a pointer to the "OK" button.
   * \since 0.1.0
   */
  [[nodiscard]] auto OkButton() -> QPushButton*;

  /**
   * Indicates whether or not the supplied line edit widget has valid input.
   *
   * \param edit the line edit widget that will be checked.
   * \return true if the line edit widget is valid; false otherwise.
   * \since 0.1.0
   */
  [[nodiscard]] auto Validate(const QLineEdit& edit) const -> QValidator::State;

 private slots:
  void OnImageButtonPressed();

  void OnWidthTextChanged();

  void OnHeightTextChanged();
};

}  // namespace tactile
