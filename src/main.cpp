#include <Qt>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "tactile_window.h"

int main(int argc, char** argv)
{
  using namespace tactile;

  QApplication app{argc, argv};

  TactileWindow window;
  window.setSurfaceType(QSurface::SurfaceType::OpenGLSurface);

  app.setParent(&window);
  window.show();

  return QGuiApplication::exec();
}