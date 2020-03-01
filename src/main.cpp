#include <QSurfaceFormat>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "tactile_application.h"
#include "tactile_window.h"

int main(int argc, char** argv)
{
  using namespace tactile;

  TactileApplication app{argc, argv};

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  TactileWindow window;
  window.showMaximized();

  return QGuiApplication::exec();
}