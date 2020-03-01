#include <Qt>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

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

  QApplication::setApplicationVersion("0.1.0");
  QApplication::setApplicationName("Tactile 2");

  TactileWindow window;
  window.showMaximized();

  return QGuiApplication::exec();
}