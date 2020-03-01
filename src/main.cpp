#include <QSurfaceFormat>
#include <Qt>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <iostream>

#include "tactile_window.h"

/*
 * Class list:
 *
 * - TileMap: representation of a map, really just a matrix of integers.
 * - TileSet: representation of tile sets (sprite sheets)
 * - Tile: representation of tiles in a tile set.
 *
 */

int main(int argc, char** argv)
{
  using namespace tactile;

  QApplication app{argc, argv};

  for (int i = 0; i < argc; ++i) {
    std::cout << "Arg" << i << ": " << argv[i] << "\n";
  }

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication::setApplicationVersion("0.1.0");
  QApplication::setApplicationName("Tactile 2");

  TactileWindow window;
  window.showMaximized();

  return QGuiApplication::exec();
}